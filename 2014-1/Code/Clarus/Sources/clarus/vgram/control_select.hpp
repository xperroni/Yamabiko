#ifndef CLARUS_VGRAM_CONTROL_SELECT_HPP
#define CLARUS_VGRAM_CONTROL_SELECT_HPP

#include <clarus/vgram/control_range.hpp>

namespace vgram {
    struct ControlSelect;
}

struct vgram::ControlSelect: public ControlRange {
    clarus::List<int> select;

    /*
    Creates a new control of given dimensions and select indices.
    */
    ControlSelect(const cv::Mat::MSize &size, const clarus::List<int> &select);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ControlSelect();

    // See control.hpp
    virtual int index(int i);
};

#endif
