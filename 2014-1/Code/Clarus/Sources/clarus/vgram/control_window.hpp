#ifndef CLARUS_VGRAM_CONTROL_WINDOW_HPP
#define CLARUS_VGRAM_CONTROL_WINDOW_HPP

#include <clarus/vgram/control_delegate.hpp>

namespace vgram {
    struct ControlWindow;
}

struct vgram::ControlWindow: public ControlDelegate {
    // Distance from window center to side.
    int radius;

    /*
    Creates a new control tied to the given parent and given window radius.
    */
    ControlWindow(Control::P control, int radius);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ControlWindow();

    // See control.hpp
    virtual uint32_t distance(const cv::Mat &input, const clarus::List<cv::Mat> &inputs);
};

#endif
