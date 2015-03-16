#include <clarus/vgram/control_range.hpp>
using clarus::List;
using vgram::ControlRange;

#include <clarus/vgram/bitstring.hpp>
using vgram::Bitstring;

#include <algorithm>
#include <cstddef>

ControlRange::ControlRange():
    looped(false)
{
    static int n = sizeof(indices) / sizeof(int);
    for (int i = 0; i < n; i++) {
        start[i] = 0;
        indices[i] = 0;
        stop[i] = 0;
    }
}

ControlRange::ControlRange(int i0, int in, int j0, int jn, int z0, int zn):
    looped(false)
{
    start[0] = i0;
    start[1] = j0;
    start[2] = z0;

    indices[0] = i0;
    indices[1] = j0;
    indices[2] = z0;

    stop[0] = in;
    stop[1] = jn;
    stop[2] = zn;
}

ControlRange::ControlRange(const cv::Mat::MSize &size) {
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;

    indices[0] = 0;
    indices[1] = 0;
    indices[2] = 0;

    stop[0] = size[0];
    stop[1] = size[1];
    stop[2] = size[2];
}

ControlRange::~ControlRange() {
    // Nothing to do.
}

uint8_t *ControlRange::bitstring(const cv::Mat &input) {
    return Bitstring::pointer(input, index(0), index(1));
}

uint8_t *ControlRange::bitstring(const List<cv::Mat> &inputs) {
    return bitstring(inputs[index(2)]);
}

uint32_t ControlRange::distance(const cv::Mat &input, const clarus::List<cv::Mat> &inputs) {
    uint8_t *b1 = bitstring(input);
    uint8_t *b2 = bitstring(inputs);
    uint32_t n = Bitstring::bytes(input);
    return Bitstring::distance(b1, b2, n);
}

int ControlRange::index(int i) {
    return indices[i];
}

bool ControlRange::more() {
    if (looped) {
        looped = false;
        return false;
    }

    return true;
}

void ControlRange::next() {
    // Number of dimensions.
    static int n = sizeof(indices) / sizeof(int);

    // The order in which indices are updated.
    static int order[3] = {1, 0, 2};

    for (int i = 0; i < n; i++) {
        int j = order[i];
        indices[j]++;
        if (indices[j] >= stop[j]) {
            indices[j] = start[j];
        }
        else {
            return;
        }
    }

    // The only case where program control reaches here is when all indices were reset.
    looped = true;
}

void ControlRange::resize(int i, int v0, int vn) {
    start[i] = v0;
    stop[i] = vn;

    indices[i] = std::min(std::max(v0, indices[i]), vn - 1);
}
