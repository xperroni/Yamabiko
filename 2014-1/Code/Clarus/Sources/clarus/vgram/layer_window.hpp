#ifndef CLARUS_VGRAM_LAYER_WINDOW_HPP
#define CLARUS_VGRAM_LAYER_WINDOW_HPP

#include <clarus/vgram/layer.hpp>

#include <boost/function.hpp>

namespace vgram {
    struct LayerWindow;
}

struct vgram::LayerWindow: public Layer {
    const size_t lo_w;

    const size_t hi_w;

    const size_t lo_s;

    const size_t hi_s;

    LayerWindow(size_t width, size_t height, size_t bytes, size_t window, size_t step);

    virtual clarus::List<cv::Mat> retrieve(const cv::Mat &input, Control &control, int top = 1);
};

#endif
