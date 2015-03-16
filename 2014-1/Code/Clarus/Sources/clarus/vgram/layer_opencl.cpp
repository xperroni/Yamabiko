#include <clarus/vgram/layer_opencl.hpp>
using clarus::List;
using vgram::Layer;
using vgram::LayerOCL;
using vgram::QueryOCL;

LayerOCL::LayerOCL(size_t width, size_t height, size_t depth):
    Layer(width, height, depth)
{
    // Nothing to do.
}

LayerOCL::~LayerOCL() {
    // Nothing to do.
}

List<cv::Mat> LayerOCL::retrieve(const cv::Mat &input, size_t top) {
    if (query.get() == NULL) {
        query.reset(new QueryOCL(*this));
    }

    return List<cv::Mat>(1, (*query)(input));
}
