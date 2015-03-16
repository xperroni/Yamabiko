#include <clarus/vgram/control_rects.hpp>
using clarus::List;
using vgram::ControlRects;

ControlRects::ControlRects() {
    // Nothing to do.
}

ControlRects::ControlRects(
    int rows, int cols,
    int sample, int records,
    const List<cv::Rect>& rects,
    const List<int> &centers
):
    ControlRange(0, rows, 0, cols, 0, sample),
    offsets(rows, cols, CV_32S)
{
    for (int i = 0, n = rects.size(); i < n; i++) {
        cv::Mat patch(offsets, rects[i]);
        patch = std::min(std::max(0, centers[i] - sample / 2), records - sample);
    }
}

ControlRects::~ControlRects() {
    // Nothing to do.
}

int ControlRects::index(int i) {
    if (i < 2) {
        return ControlRange::index(i);
    }

    return indices[2] + offsets.at<int>(indices[0], indices[1]);
}

bool ControlRects::more() {
    if (offsets.empty()) {
        return false;
    }

    return ControlRange::more();
}
