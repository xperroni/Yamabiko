#ifndef CIGHT_DRIFT_REDUCE_HPP
#define CIGHT_DRIFT_REDUCE_HPP


#include <opencv2/opencv.hpp>
#include <boost/function.hpp>

namespace cight {
    int reduce_wta(const cv::Mat &responses);

    class reduce_slip;
}

class cight::reduce_slip {
    int last;

public:
    reduce_slip();

    int operator () (const cv::Mat &responses);
};

#endif
