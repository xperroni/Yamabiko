#include <clarus/vgram/control_slider.hpp>
using vgram::ControlSlider;

#include <algorithm>

ControlSlider::ControlSlider():
    ControlRange(0, 0, 0, 0, 0, 0),
    reach(0),
    breadth(0),
    pace(0),
    step(0)
{
    // Nothing to do.
}

ControlSlider::ControlSlider(int rows, int cols, int row_0, int row_n, int _breadth, int _pace):
    ControlRange(0, rows, 0, cols, row_0, row_0 + breadth),
    reach(row_n),
    breadth(_breadth),
    pace(_pace),
    step(0)
{
    // Nothing to do.
}

ControlSlider::~ControlSlider() {
    // Nothing to do.
}

void ControlSlider::centralize(int center) {
    int index = std::min(std::max(0, center - breadth / 2), reach - breadth);
    start[2] = index;
    indices[2] = index;
    stop[2] = index + breadth;
}

void ControlSlider::increment() {
    // Stop incrementing after the last index comes into range.
    if (stop[2] >= reach) {
        return;
    }

    step++;
    if (step >= pace) {
        step = 0;
        start[2]++;
        stop[2]++;
    }
}
