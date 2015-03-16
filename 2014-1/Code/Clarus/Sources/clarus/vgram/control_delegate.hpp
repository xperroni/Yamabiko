#ifndef CLARUS_VGRAM_CONTROL_DELEGATE_HPP
#define CLARUS_VGRAM_CONTROL_DELEGATE_HPP

#include <clarus/vgram/control.hpp>

namespace vgram {
    struct ControlDelegate;
}

struct vgram::ControlDelegate: public Control {
    Control::P control;

    /*
    Creates a new delegate bound to the given control.
    */
    ControlDelegate(Control::P control);

    // See control.hpp
    virtual uint8_t *bitstring(const cv::Mat &input);

    // See control.hpp
    virtual uint8_t *bitstring(const clarus::List<cv::Mat> &inputs);

    // See control.hpp
    virtual uint32_t distance(const cv::Mat &input, const clarus::List<cv::Mat> &inputs);

    // See control.hpp
    virtual int index(int i);

    // See control.hpp
    virtual bool more();

    // See control.hpp
    virtual void next();

    // See control.hpp
    virtual void resize(int i, int v0, int vn);
};

#endif // CLARUS_VGRAM_CONTROL_DELEGATE_HPP
