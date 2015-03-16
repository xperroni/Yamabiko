#include <clarus/vgram/control_select.hpp>
using clarus::List;
using vgram::ControlSelect;

ControlSelect::ControlSelect(const cv::Mat::MSize &size, const List<int> &_select):
    ControlRange(0, size[0], 0, size[1], 0, _select.size()),
    select(_select)
{
    // Nothing to do.
}

ControlSelect::~ControlSelect() {
    // Nothing to do.
}

int ControlSelect::index(int i) {
    if (i < 2) {
        return ControlRange::index(i);
    }

    return select[indices[2]];
}
