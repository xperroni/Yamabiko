#include <clarus/vgram/layer_window_rl.hpp>
using clarus::List;
using vgram::LayerWindowRL;

#include <clarus/vision/cvmat.hpp>

LayerWindowRL::LayerWindowRL(
    size_t width, size_t height, int type,
    size_t _capacity, size_t window, size_t step):
    LayerWindow(width, height, cvmat::type::length(type), window, step),
    capacity(_capacity)
{
    // Nothing to do.
}

inline size_t topleft(size_t i, size_t n, size_t lo, size_t hi) {
    return std::min(std::max((size_t) 0, i - lo), n - hi);
}

void LayerWindowRL::record(const cv::Mat &input, const cv::Mat &output) {
    static cv::RNG rng;

    if (inputs.size() < capacity) {
        Layer::record(input, output);
        cv::Mat miss(size(), CV_8U, cv::Scalar(0));
        misses.append(miss);
        return;
    }

    List<cv::Mat> response = Layer::retrieve(input);
    cv::Mat o = response[0];
    cv::Mat l = response[2];

    size_t rows = o.rows;
    size_t cols = o.cols;

    for (size_t i = lo_s; i < rows; i+= hi_s) {
        size_t i_w = topleft(i, rows, lo_w, hi_w);
        for (size_t j = lo_s; j < cols; j+= hi_s) {
            size_t j_w = topleft(j, cols, lo_w, hi_w);

            int z = l.at<int>(i, j);
            uint8_t &m = misses[z].at<uint8_t>(i, j);
            if (o.at<uint8_t>(i, j) != output.at<uint8_t>(i, j)) {
                m++;
                if (m > rng(253) + 1) {
                    cv::Rect roi(j_w, i_w, hi_w, hi_w);

                    cv::Mat i1(input, roi);
                    cv::Mat i2(inputs[z], roi);
                    i1.copyTo(i2);

                    cv::Mat o1(output, roi);
                    cv::Mat o2(outputs[z], roi);
                    o1.copyTo(o2);

                    m--;
                }
            }
            else if (m > 0) {
                m /= 2;
            }
        }
    }
/*
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            int z = l.at<int>(i, j);
            uint8_t &m = misses[z].at<uint8_t>(i, j);
            if (o.at<uint8_t>(i, j) != output.at<uint8_t>(i, j)) {
                m++;
                if (m >= (uint8_t) rng) {
                    cvmat::copy(inputs[z], i, j, input, i, j);
                    cvmat::copy(outputs[z], i, j, output, i, j);
                    m /= 2;
                }
            }
            else if (m > 0) {
                m /= 2;
            }
        }
    }
*/
}
