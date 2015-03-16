#include <cight/drift_reduce.hpp>
using cight::reduce_slip;

reduce_slip::reduce_slip():
    last(INT_MIN)
{
    // Nothing to do.
}

inline double drag(int y, int i, double g) {
    double d = fabs(y - i);
    return (d == 0 ? g : g / d);
}

inline cv::Mat normalize(const cv::Mat &responses) {
    double minVal = 0.0;
    cv::minMaxLoc(responses, &minVal);
    cv::Mat normal = responses - minVal;

    double maxVal = 0.0;
    cv::minMaxLoc(responses, NULL, &maxVal);
    normal /= maxVal;

    return normal;
}

inline int correction(int y, const cv::Mat &responses) {
    int cols = responses.cols;

    int yl = y;
    double rl = responses.at<double>(0, y);
    for (int i = y - 1; i >= 0; i--) {
        double r = responses.at<double>(0, i);
        if (r < rl) {
            break;
        }

        if (r > rl) {
            yl = i;
            rl = r;
        }
    }

    int yr = y;
    double rr = responses.at<double>(0, y);
    for (int i = y + 1; i < cols; i++) {
        double r = responses.at<double>(0, i);
        if (r < rr) {
            break;
        }

        if (r > rr) {
            yr = i;
            rr = r;
        }
    }

    if (drag(y, yl, rl) > drag(y, yr, rr)) {
        return yl;
    }
    else {
        return yr;
    }
}

int reduce_slip::operator () (const cv::Mat &responses) {
    if (last != INT_MIN) {
        last = correction(last, normalize(responses));
    }
    else {
        last = reduce_wta(responses) + responses.cols / 2;
    }

    return last - responses.cols / 2;
}

int cight::reduce_wta(const cv::Mat &responses) {
    cv::Point maxLoc;
    cv::minMaxLoc(responses, NULL, NULL, NULL, &maxLoc);
    return maxLoc.x - responses.cols / 2;
}
