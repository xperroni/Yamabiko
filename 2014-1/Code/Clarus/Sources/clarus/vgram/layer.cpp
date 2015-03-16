#include <clarus/vgram/layer.hpp>
using clarus::List;
using vgram::Control;
using vgram::Layer;

#include <clarus/core/domain.hpp>

#include <clarus/vgram/control_range.hpp>
using vgram::ControlRange;

Layer::Layer(size_t width, size_t height, size_t bytes):
    size(domain::size3D(width, height, bytes))
{
    // Nothing to do.
}

Layer::~Layer() {
    // Nothing to do.
}

void Layer::record(const cv::Mat &input, const cv::Mat &output) {
    inputs.append(input);
    outputs.append(output);
}

List<cv::Mat> Layer::retrieve(const cv::Mat &input, int top) {
    ControlRange all(0, size[0], 0, size[1], 0, inputs.size());
    return retrieve(input, all, top);
}

inline int bigger(uint32_t dz, uint32_t *dk, int n) {
    int w = 0;
    uint32_t dw = *dk;
    for (int k = 1; k < n; k++) {
        if (dw < *(++dk)) {
            dw = *dk;
            w = k;
        }
    }

    return (dz < dw ? w : n);
}

List<cv::Mat> Layer::retrieve(const cv::Mat &input, Control &control, int top) {
    const int sizes[] = {size[0], size[1], top};

    cv::Mat o(3, sizes, CV_8U, cv::Scalar(0));
    cv::Mat d(3, sizes, CV_32S, cv::Scalar(INT_MAX));
    cv::Mat l(3, sizes, CV_32S, cv::Scalar(0));

    for (; control.more(); control.next()) {
        size_t i = control.index(0);
        size_t j = control.index(1);

        uint32_t dz = control.distance(input, inputs);
        uint32_t *dk = &(d.at<uint32_t>(i, j, 0));
        int w = bigger(dz, dk, top);
        if (w < top) {
            size_t z = control.index(2);
            o.at<uint8_t>(i, j, w) = outputs[z].at<uint8_t>(i, j);
            d.at<uint32_t>(i, j, w) = dz;
            l.at<int32_t>(i, j, w) = z;
        }
    }

    List<cv::Mat> response;
    response.append(o);
    response.append(d);
    response.append(l);
    return response;
}

uint32_t Layer::total() const {
    return size[0] * size[1] * size[2];
}
