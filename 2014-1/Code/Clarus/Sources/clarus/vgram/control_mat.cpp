#include <clarus/vgram/control_mat.hpp>
using vgram::ControlMat;

ControlMat::ControlMat() {
    // Nothing to do.
}

ControlMat::ControlMat(const cv::Mat &_data):
    ControlRange(0, _data.size[0], 0, _data.size[1], 0, _data.size[2]),
    data(_data)
{
    // Nothing to do.
}

ControlMat::~ControlMat() {
    // Nothing to do.
}

int ControlMat::index(int i) {
    if (i < 2) {
        return ControlRange::index(i);
    }

    return data.at<int>(indices[0], indices[1], indices[2]);
}

bool ControlMat::more() {
    if (data.empty()) {
        return false;
    }

    return ControlRange::more();
}
