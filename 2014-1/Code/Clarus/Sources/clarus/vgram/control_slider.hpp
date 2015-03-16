#ifndef CLARUS_VGRAM_CONTROL_SLIDER_HPP
#define CLARUS_VGRAM_CONTROL_SLIDER_HPP

#include <clarus/vgram/control_range.hpp>

namespace vgram {
    struct ControlSlider;
}

struct vgram::ControlSlider: public ControlRange {
    // One past the last position covered by this range.
    int reach;

    // Size of the slider aperture.
    int breadth;

    // Pace of range increment.
    int pace;

    // Position over the current step.
    int step;

    /*
    Creates a new control range.
    */
    ControlSlider();

    /*
    Creates a new control range of given initial and last index, range length and pace.
    */
    ControlSlider(int rows, int cols, int row_0, int row_n, int breadth, int pace = 0);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ControlSlider();

    /*
    Adjusts the index so the range is centered on the given position.
    */
    void centralize(int center);

    /*
    Increments the range index, conditional to the size of the pace and current step value.
    */
    void increment();
};

#endif // CLARUS_VGRAM_CONTROL_RANGE_HPP
