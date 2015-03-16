#include <clarus/vgram/control_focus.hpp>
using clarus::List;
using vgram::ControlFocus;

#include <clarus/vgram/bitstring.hpp>
using vgram::Bitstring;

ControlFocus::ControlFocus(
    const cv::Mat::MSize &size,
    const cv::Point2i &focus,
    const cv::Point3i &center,
    int around
):
    ControlDelegate(Control::P(new ControlRange(size))),
    range((ControlRange*) control.get())
{
    init(focus, center, around);
}

ControlFocus::ControlFocus(
    Control::P _range,
    const cv::Point2i &focus,
    const cv::Point3i &center,
    int around
):
    ControlDelegate(Control::P(_range)),
    range((ControlRange*) _range.get())
{
    init(focus, center, around);
}

ControlFocus::~ControlFocus() {
    // Nothing to do.
}

void ControlFocus::init(const cv::Point2i &focus, const cv::Point3i &center, int around) {
    offsets[0] = center.y - focus.y;
    offsets[1] = center.x - focus.x;

    // Correct 2D range limits to avoid overflow.
    for (int i = 0; i < 2; i++) {
        int d = offsets[i];
        if (d < 0) {
            range->start[i] -= d;
            range->indices[i] = range->start[i];
        }
        else {
            range->stop[i] -= d;
        }
    }

    if (around > 0) {
        int z0 = std::max(center.z - around, 0);
        int zn = std::min(center.z + around, range->stop[2]);
        range->indices[2] = z0;
        range->start[2] = z0;
        range->stop[2] = zn;
    }
}

uint8_t *ControlFocus::bitstring(const List<cv::Mat> &inputs) {
    int i = index(0) + offsets[0];
    int j = index(1) + offsets[1];
    int z = index(2);

    return Bitstring::pointer(inputs[z], i, j);
}
