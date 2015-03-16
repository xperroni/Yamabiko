#include <clarus/vgram/control_window.hpp>
using clarus::List;
using vgram::ControlWindow;

#include <clarus/vgram/bitstring.hpp>
using vgram::Bitstring;

ControlWindow::ControlWindow(Control::P control, int _radius):
    ControlDelegate(control),
    radius(_radius)
{
    // Nothing to do.
}

ControlWindow::~ControlWindow() {
    // Nothing to do.
}

uint32_t ControlWindow::distance(const cv::Mat &input, const List<cv::Mat> &inputs) {
    int i = index(0);
    int j = index(1);
    int z = index(2);

    int side = 2 * radius + 1;
    int u0 = std::min(std::max(0, (int) i - radius), input.size[0] - side);
    int v0 = std::min(std::max(0, (int) j - radius), input.size[1] - side);

    const cv::Mat &memory = inputs[z];
    cv::Rect roi(v0, u0, side, side);
    return Bitstring::distance(input, memory, roi);
}
