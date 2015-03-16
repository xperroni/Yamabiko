#ifndef CLARUS_VGRAM_LAYER_WINDOW_RL_HPP
#define CLARUS_VGRAM_LAYER_WINDOW_RL_HPP

#include <clarus/vgram/layer_window.hpp>

namespace vgram {
    struct LayerWindowRL;
}

struct vgram::LayerWindowRL: public LayerWindow {
    // Miss count for each stored memory in the layer.
    clarus::List<cv::Mat> misses;

    // Maximum number of records that can be stored in this layer.
    size_t capacity;

    /*
    Creates a new reinforcement-learning windowed layer.
    */
    LayerWindowRL(size_t width, size_t height, int type, size_t capacity, size_t window, size_t step);

    // See layer.hpp
    virtual void record(const cv::Mat &input, const cv::Mat &output);
};

#endif
