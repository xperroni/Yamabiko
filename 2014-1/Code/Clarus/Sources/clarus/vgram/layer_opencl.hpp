#ifndef CLARUS_VGRAM_LAYER_OPEN_CL_HPP
#define CLARUS_VGRAM_LAYER_OPEN_CL_HPP

#include <clarus/vgram/layer.hpp>
#include <clarus/vgram/query_opencl.hpp>

#include <boost/smart_ptr.hpp>

#include <opencv2/opencv.hpp>

namespace vgram {
    class LayerOCL;
}

class vgram::LayerOCL: public Layer {
    boost::shared_ptr<QueryOCL> query;

public:
    /*
    Creates a new layer of given dimensions.
    */
    LayerOCL(size_t width, size_t height, size_t depth);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~LayerOCL();

    /*
    Query this layer for the given input.
    */
    virtual clarus::List<cv::Mat> retrieve(const cv::Mat &input, size_t top = 1);
};

#endif
