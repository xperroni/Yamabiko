#ifndef CLARUS_VGRAM_CONTROL_RECTS_HPP
#define CLARUS_VGRAM_CONTROL_RECTS_HPP

#include <clarus/vgram/control_range.hpp>

namespace vgram {
    struct ControlRects;
}

struct vgram::ControlRects: public ControlRange {
    // Matrix of value offsets.
    cv::Mat offsets;

    /*
    Default constructor.
    */
    ControlRects();

    /*
    Creates a new control dividing a layer memory into a set of range areas.
    */
    ControlRects(
        int rows, int cols,
        int sample, int records,
        const clarus::List<cv::Rect>& rects,
        const clarus::List<int> &centers
    );

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ControlRects();

    // See control.hpp
    virtual int index(int i);

    // See control.hpp
    virtual bool more();
};

#endif
