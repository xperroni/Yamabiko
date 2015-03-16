#include <clarus/vgram/layer_window.hpp>
using clarus::List;
using vgram::LayerWindow;

#include <clarus/vision/colors.hpp>
#include <clarus/vision/images.hpp>

LayerWindow::LayerWindow(size_t width, size_t height, size_t bytes, size_t window, size_t step):
    Layer(width, height, bytes),
    lo_w(window),
    hi_w(2 * window + 1),
    lo_s(step),
    hi_s(2 * step + 1)
{
    // Nothing to do.
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

inline size_t topleft(size_t i, size_t n, size_t lo, size_t hi) {
    return std::min(std::max((size_t) 0, i - lo), n - hi);
}

inline cv::Mat integral_distances(const cv::Mat &input, const cv::Mat &memory) {
    cv::Mat d(input.size(), CV_32F, cv::Scalar::all(0));
    cv::Mat diffs = images::absdiff(input, memory);
    List<cv::Mat> channels = colors::channels(diffs);
    for (int i = 0, n = channels.size(); i < n; i++) {
        cv::add(d, channels[i], d, cv::noArray(), CV_32F);
    }

    cv::Mat integral;
    cv::integral(d, integral, CV_32F);
    return integral;
}

List<cv::Mat> LayerWindow::retrieve(const cv::Mat &input, Control &control, int top) {
    size_t rows = size[0];
    size_t cols = size[1];
    cv::Mat o(rows, cols, CV_8U, cv::Scalar(0));
    cv::Mat d(rows, cols, CV_32S, cv::Scalar(INT_MAX));
    cv::Mat l(rows, cols, CV_32S, cv::Scalar(0));

    for (size_t z = 0, n = inputs.size(); z < n; z++) {
        cv::Mat intl_d = integral_distances(input, inputs[z]);
        for (size_t i = lo_s; i < rows; i+= hi_s) {
            //size_t y_s = topleft(i, rows, lo_w, hi_w);
            size_t i1_w = topleft(i, rows, lo_w, hi_w);
            size_t i2_w = i1_w + hi_w;

            for (size_t j = lo_s; j < cols; j+= hi_s) {
                size_t j1_w = topleft(j, cols, lo_w, hi_w);
                size_t j2_w = j1_w + hi_w;

                int e =
                    intl_d.at<float>(i1_w, j1_w) + intl_d.at<float>(i2_w, j2_w) -
                    intl_d.at<float>(i1_w, j2_w) - intl_d.at<float>(i2_w, j1_w)
                ;

                if (e < d.at<int>(i, j)) {
                    //size_t x_s = topleft(j, cols, lo_s, hi_s);
                    //cv::Rect roi_s(x_s, y_s, hi_s, hi_s);
                    cv::Rect roi_s(j1_w, i1_w, hi_w, hi_w);
                    cv::Mat ds(d, roi_s);

                    cv::Mat mask = (ds > e);
                    ds.setTo(e, mask);

                    cv::Mat o1(outputs[z], roi_s);
                    cv::Mat o2(o, roi_s);
                    o1.copyTo(o2, mask);

                    cv::Mat ls(l, roi_s);
                    ls.setTo(z, mask);
                }
            }
        }
    }

    List<cv::Mat> response;
    response.append(o);
    response.append(d);
    response.append(l);
    return response;
}
