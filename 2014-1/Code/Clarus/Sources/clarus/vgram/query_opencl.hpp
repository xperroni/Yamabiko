#ifndef CLARUS_VGRAM_QUERY_OPEN_CL_HPP
#define CLARUS_VGRAM_QUERY_OPEN_CL_HPP

#include <clarus/vgram/layer.hpp>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <opencv2/opencv.hpp>

namespace vgram {
    struct QueryOCL;
}

struct vgram::QueryOCL {
    uint32_t width;

    uint32_t height;

    uint32_t depth;

    // OpenCL context.
    cl::Context context;

    // OpenCL devices available on the current platform.
    clarus::List<cl::Device> devices;

    // Maximum image depth for the default device.
    size_t depthMax;

    // Command queue to the default device.
    cl::CommandQueue queue;

    // OpenCL program sources.
    cl::Program::Sources sources;

    // OpenCL compiled program.
    cl::Program program;

    // Access interface to kernel query_layer().
    cl::Kernel query_layer;

    // Dimensions of the global work-item collection for the query kernel.
    cl::NDRange queryGlobal;

    // Access interface to kernel setup_d0().
    cl::Kernel setup_d0;

    // Dimensions of the global work-item collection for the setup kernel.
    cl::NDRange setupGlobal;

    // 2D matrix of query bit strings.
    cl::Buffer b;

    // 3D matrix of known bit strings.
    cl::Buffer I;

    // 3D matrix of associated outputs.
    cl::Buffer O;

    // Query output.
    cl::Buffer y;

    // Distances between input bit strings and best matches.
    cl::Buffer d;

    /*
    Creates a new OpenCL network query for a given VG-RAM layer.
    */
    QueryOCL(const Layer &layer);


    virtual ~QueryOCL();

    cv::Mat operator () (const cv::Mat &input);
};

#endif // CLARUS_VGRAM_QUERY_OPEN_CL_HPP
