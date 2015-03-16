#include <clarus/vgram/control_delegate.hpp>
using clarus::List;
using vgram::ControlDelegate;

ControlDelegate::ControlDelegate(Control::P _control):
    control(_control)
{
    // Nothing to do.
}

uint8_t *ControlDelegate::bitstring(const cv::Mat &input) {
    return control->bitstring(input);
}

uint8_t *ControlDelegate::bitstring(const List<cv::Mat> &inputs) {
    return control->bitstring(inputs);
}

uint32_t ControlDelegate::distance(const cv::Mat &input, const clarus::List<cv::Mat> &inputs) {
    return control->distance(input, inputs);
}

int ControlDelegate::index(int i) {
    return control->index(i);
}

bool ControlDelegate::more() {
    return control->more();
}

void ControlDelegate::next() {
    return control->next();
}

void ControlDelegate::resize(int i, int v0, int vn) {
    control->resize(i, v0, vn);
}
