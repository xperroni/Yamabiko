#ifndef DEJAVU_SENSOR_STREAM_HPP
#define DEJAVU_SENSOR_STREAM_HPP

#include <opencv2/opencv.hpp>

namespace cight {
    class SensorStream;
}

class cight::SensorStream {
public:
    struct S {
        cv::Mat image;

        double x;

        S(const cv::Mat &_image, double _x):
            image(_image),
            x(_x)
        {
            // Nothing to do.
        }
    };

    virtual S operator () () = 0;

    virtual bool active() = 0;
};

#endif
