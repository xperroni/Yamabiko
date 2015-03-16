#include <clarus/vgram/query_opencl.hpp>
using clarus::List;
using vgram::Layer;
using vgram::QueryOCL;

#include <cstdarg>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

static cl_context_properties *get_context_properties() {
    static cl_context_properties properties[3] = {CL_CONTEXT_PLATFORM, 0, 0};

    if (properties[1] == 0) {
        // Get available platforms.
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        // Get the ID of the default platform.
        properties[1] = (cl_context_properties) (platforms[0])();
    }

    return properties;
}

static cl::Program::Sources load_sources() {
    std::ifstream file("./layer_opencl.cl");
    std::string source(
        std::istreambuf_iterator<char>(file),
        (std::istreambuf_iterator<char>()) // Extra parentheses needed, no idea why
    );

    std::cout << "Source file:" << std::endl << source << std::endl;

    return cl::Program::Sources(1, std::make_pair(source.c_str(), source.length() + 1));
}

static cl::Program load_program(
    cl::Context &context,
    List<cl::Device> &devices,
    cl::Program::Sources &sources
) {
    cl::Program program(context, sources);
    program.build(*devices);
    return program;
}

template<size_t N> static cl::size_t<N> make_size_t(size_t s0, ...) {
    cl::size_t<N> s;
    s.push_back(s0);

    va_list args;
    va_start(args, s0);

    for (size_t i = 1; i < N; i++) {
        size_t v = va_arg(args, size_t);
        s.push_back(v);
    }

    va_end(args);

    return s;
}

static void fillBuffer3D(cl::CommandQueue &queue, cl::Buffer &buffer, const List<cv::Mat> &data) {
    size_t offset = 0;
    size_t n = data.size();
    for (size_t i = 0; i < n; i++) {
        const cv::Mat &slice = data[i];
        size_t m = slice.total() * slice.elemSize();
        queue.enqueueWriteBuffer(
            buffer, CL_TRUE,
            offset, m,
            slice.data
        );

        offset += m;
    }
}

QueryOCL::QueryOCL(const Layer &layer):
    width(layer.size[1]),
    height(layer.size[0]),
    depth(layer.inputs.size()),

    context(CL_DEVICE_TYPE_GPU, get_context_properties()),
    devices(context.getInfo<CL_CONTEXT_DEVICES>()),
    depthMax(devices[0].getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH>()),
    queue(context, devices[0]),
    sources(load_sources()),
    program(load_program(context, devices, sources)),

    query_layer(program, "query_layer"),
    queryGlobal(width, height, depth),

    setup_d0(program, "setup_d0"),
    setupGlobal(width, height),

    b(
        context,
        CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR,
        width * height * 4 * sizeof(uint32_t)
    ),

    I(
        context,
        CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR,
        width * height * depth * 4 * sizeof(uint32_t)
    ),

    O(
        context,
        CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR,
        width * height * depth * sizeof(uint8_t)
    ),

    y(
        context,
        CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR,
        width * height * sizeof(uint32_t)
    ),

    d(
        context,
        CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR,
        width * height * sizeof(uint32_t)
    )
{
    fillBuffer3D(queue, I, layer.inputs);
    fillBuffer3D(queue, O, layer.outputs);

    query_layer.setArg(0, b);
    query_layer.setArg(1, I);
    query_layer.setArg(2, O);
    query_layer.setArg(3, y);
    query_layer.setArg(4, d);

    setup_d0.setArg(0, d);
}

QueryOCL::~QueryOCL() {
    // Nothing to do.
}

cv::Mat QueryOCL::operator () (const cv::Mat &query) {
    queue.enqueueNDRangeKernel(setup_d0, cl::NullRange, setupGlobal, cl::NullRange);

    size_t n = query.total();

    queue.enqueueWriteBuffer(
        b, CL_TRUE,
        0, n * query.elemSize(),
        query.data
    );

    queue.enqueueNDRangeKernel(query_layer, cl::NullRange, queryGlobal, cl::NullRange);

    cv::Mat out(query.size(), CV_32S);
    queue.enqueueReadBuffer(
        y, CL_TRUE,
        0, n * out.elemSize(),
        out.data
    );

    return out;
}
