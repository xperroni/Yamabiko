#ifndef CLARUS_VGRAM_CONTROL_FOCUS_HPP
#define CLARUS_VGRAM_CONTROL_FOCUS_HPP

#include <clarus/vgram/control_delegate.hpp>
#include <clarus/vgram/control_range.hpp>

namespace vgram {
    struct ControlFocus;
}

struct vgram::ControlFocus: public ControlDelegate {
    // Range of search within memory.
    ControlRange *range;

    // 2D offsets between field of view and memory.
    int offsets[2];

    /*
    Creates a new control object relating focus points in the field of view and memory.
    */
    ControlFocus(
        const cv::Mat::MSize &size,
        const cv::Point2i &focus,
        const cv::Point3i &center,
        int around = 0
    );

    /*
    Creates a new control object relating focus points in the field of view and memory.
    */
    ControlFocus(
        Control::P range,
        const cv::Point2i &focus,
        const cv::Point3i &center,
        int around = 0
    );

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ControlFocus();

    // See control.hpp
    virtual uint8_t *bitstring(const clarus::List<cv::Mat> &inputs);

private:
    void init(const cv::Point2i &focus, const cv::Point3i &center, int around);
};

#endif
