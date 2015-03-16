#ifndef CLARUS_VGRAM_CONTROL_MAT_HPP
#define CLARUS_VGRAM_CONTROL_MAT_HPP

#include <clarus/vgram/control_range.hpp>

#include <opencv2/opencv.hpp>

namespace vgram {
    struct ControlMat;
}

struct vgram::ControlMat: public ControlRange {
    cv::Mat data;

    /*
    Creates a new matrix control object.
    */
    ControlMat();

    /*
    Creates a new control object bound to the given matrix.
    */
    ControlMat(const cv::Mat &data);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ControlMat();

    // See control.hpp
    virtual int index(int i);

    // See control.hpp
    virtual bool more();
};

#endif // CLARUS_VGRAM_CONTROL_MAT_HPP
