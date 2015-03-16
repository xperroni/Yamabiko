#include <clarus/vgram/layer_extensions.hpp>
using clarus::List;
using vgram::Layer;

#include <clarus/vgram/bitstring.hpp>
using vgram::Bitstring;

#include <clarus/vgram/control_range.hpp>
using vgram::Control;
using vgram::ControlRange;

#include <clarus/vgram/layer_extensions.hpp>

int vgram::relate(const Layer &layer, const cv::Mat &input, int r0, int rn) {
    const List<cv::Mat> &inputs = layer.inputs;
    uint32_t bytes = layer.total();
    uint8_t *buffer = input.data;

    int index = 0;
    uint32_t least = std::numeric_limits<uint32_t>::max();

    for (int i = r0; i < rn; i++) {
        uint32_t d = Bitstring::distance(buffer, inputs[i].data, bytes);
        if (d < least) {
            index = i;
            least = d;
        }
    }

    return index;
}

cv::Point3i vgram::relate(const Layer &layer, const cv::Mat &input, const cv::Point2i &focus) {
    ControlRange all(0, 1, 0, 1, 0, layer.inputs.size());
    return vgram::relate(layer, input, focus, all);
}

cv::Point3i vgram::relate(
    const Layer &layer,
    const cv::Mat &input,
    const cv::Point2i &focus,
    Control &control
) {
    const List<cv::Mat> &inputs = layer.inputs;
    uint8_t *buffer = Bitstring::pointer(input, focus.y, focus.x);

    cv::Point3i center;
    int least = std::numeric_limits<int>::max();
    for (; control.more(); control.next()) {
        size_t z = control.index(2);
        cv::Point3i closest = Bitstring::closest(buffer, inputs[z]);
        if (closest.z < least) {
            least = closest.z;
            center.x = closest.x;
            center.y = closest.y;
            center.z = z;
        }
    }

    return center;
}

List<int> vgram::relate(const Layer &layer, const cv::Mat &input, int n) {
    const List<cv::Mat> &inputs = layer.inputs;
    uint32_t bytes = layer.total();
    uint8_t *buffer = input.data;

    List<int> indices(n, 0);
    List<uint32_t> least(n, std::numeric_limits<uint32_t>::max());
    for (int i = 0, m = inputs.size(); i < m; i++) {
        int k = 0;
        uint32_t worst = least[0];
        for (int j = 1; j < n; j++) {
            uint32_t e = least[j];
            if (worst < e) {
                worst = e;
                k = j;
            }
        }

        uint32_t d = Bitstring::distance(buffer, inputs[i].data, bytes);
        if (d < worst) {
            indices[k] = i;
            least[k] = d;
        }
    }

    return indices;
}

List<int> vgram::relate(const Layer &layer, const cv::Mat &input, const List<cv::Rect> &rects) {
    List<int> indices(rects.size(), 0);
    const List<cv::Mat> &inputs = layer.inputs;
    List<uint32_t> distances(rects.size(), INT_MAX);
    for (int i = 0, m = inputs.size(); i < m; i++) {
        const cv::Mat &memory = inputs[i];
        for (int j = 0, n = rects.size(); j < n; j++) {
            const cv::Rect &rect = rects[j];
            uint32_t &least = distances[j];
            uint32_t d = Bitstring::distance(input, memory, rect);
            if (d < least) {
                indices[j] = i;
                least = d;
            }
        }
    }

    return indices;
}
