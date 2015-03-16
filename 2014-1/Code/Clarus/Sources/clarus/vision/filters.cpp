/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#include <clarus/vision/filters.hpp>

#include <clarus/core/list.hpp>
using clarus::List;
using clarus::ListIterator;
using clarus::ListIteratorConst;

#include <clarus/core/domain.hpp>

#include <clarus/model/point.hpp>
using clarus::Point;
using clarus::Point3D;
using clarus::distance2;

#include <clarus/vision/colors.hpp>
#include <clarus/vision/cvmat.hpp>
#include <clarus/vision/gaussian.hpp>

cv::Mat filter::channelwise(Filter f, const cv::Mat &image) {
    List<cv::Mat> channels;
    cv::split(image, *channels);
    for (ListIterator<cv::Mat> i(channels); i.more();) {
        cv::Mat &channel = i.next();
        channel = f(channel);
    }

    cv::Mat result;
    cv::merge(*channels, result);
    return result;
}

cv::Mat filter::convolve(const cv::Mat &image, const cv::Mat &kernel) {
    List<cv::Mat> channels;
    List<cv::Mat> kernels;
    cv::split(image, *channels);
    cv::split(kernel, *kernels);

    cv::Mat result = cv::Mat::zeros(image.size(), CV_64F);
    for (ListIterator<cv::Mat> i(channels), j(kernels); i.more();) {
        cv::Mat channel = cvmat::convert(i.next(), CV_64F);
        cv::Mat kernel = cvmat::convert(j.next(), CV_64F);

        cv::Mat output;
        cv::filter2D(channel, output, -1, kernel);
        result += output;
    }

    return result;
}

cv::Mat filter::energy(const cv::Mat &data, size_t w) {
    CHANNEL_WISE(energy, data, w);

    cv::Mat e(data.size(), CV_64F);

    size_t u = w / 2;
    for (size_t i = 0, m = data.rows; i < m; i++) {
        for (size_t j = 0, n = data.cols; j < n; j++) {
            size_t x = (j < u ? j : j - u);
            size_t y = (i < u ? i : i - u);
            size_t width = (x + w > n ? n - x : w);
            size_t height = (y + w > m ? m - y : w);

            cv::Rect roi(x, y, width, height);
            cv::Mat patch(data, roi);
            e.at<float>(i, j) = cv::sum(patch)[0];
        }
    }

    return e;
}

cv::Mat filter::gamma(const cv::Mat &src, double g) {
    CHANNEL_WISE(gamma, src, g);

    cv::Mat data;
    int type = src.type();
    if (type != CV_64F) {
        src.convertTo(data, CV_64F);
    }
    else {
        data = src;
    }

    cv::Mat dst;
    if (g > 0) {
        cv::pow(data, g, dst);
    }
    else {
        cv::log(data + 1, dst);
    }

    return dst;
}

cv::Mat filter::gradients(const cv::Mat &l) {
    int rows = l.rows;
    int cols = l.cols;

    /// Generate grad_x and grad_y
    cv::Mat grad_x = cv::Mat::zeros(rows, cols, CV_16S);
    cv::Mat grad_y = cv::Mat::zeros(rows, cols, CV_16S);

    /// Gradient X
    cv::Sobel(l, grad_x, CV_16S, 1, 0, 3);

    /// Gradient Y
    cv::Sobel(l, grad_y, CV_16S, 0, 1, 3);

    cv::Mat out(l.size(), CV_8U, cv::Scalar::all(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double rads = atan2(fabs(grad_y.at<int16_t>(i, j)), grad_x.at<int16_t>(i, j));
            int hue = ((int) (255 * (rads / M_PI))) % 256;
/*
            int hue = (
                0.0 * M_PI <= rads && rads < 0.5 * M_PI ? 0 :
                0.5 * M_PI <= rads && rads < 1.0 * M_PI ? 64 :
                1.0 * M_PI <= rads && rads < 1.5 * M_PI ? 128 :
                1.5 * M_PI <= rads && rads < 2.0 * M_PI ?  255
            );
*/
            //out.at<cv::Vec3b>(i, j) = cv::Vec3b(hue, 127, 255);
            out.at<uint8_t>(i, j) = hue;
        }
    }

    //cv::cvtColor(out.clone(), out, CV_HLS2BGR);

    return out;
}

List<cv::Mat> filter::laws(const cv::Mat &data, size_t w) {
    static const Kernel<5> *laws_masks[] = {
        new Kernel<5>( // E5E5
              1.0,  2.0,  0.0, -2.0, -1.0,
              2.0,  4.0,  0.0, -4.0, -2.0,
              0.0,  0.0,  0.0,  0.0,  0.0,
             -2.0, -4.0,  0.0,  4.0,  2.0,
             -1.0, -2.0,  0.0,  2.0,  1.0
        ),

        new Kernel<5>( // E5R5
              -1.0,   4.0,  -6.0,   4.0,  -1.0,
              -2.0,   8.0, -12.0,   8.0,  -2.0,
               0.0,   0.0,   0.0,   0.0,   0.0,
               2.0,  -8.0,  12.0,  -8.0,   2.0,
               1.0,  -4.0,   6.0,  -4.0,   1.0
        ),

        new Kernel<5>( // E5L5
              -1.0,  -4.0,  -6.0,  -4.0,  -1.0,
              -2.0,  -8.0, -12.0,  -8.0,  -2.0,
               0.0,   0.0,   0.0,   0.0,   0.0,
               2.0,   8.0,  12.0,   8.0,   2.0,
               1.0,   4.0,   6.0,   4.0,   1.0
        ),

        new Kernel<5>( // E5S5
              1.0,  0.0, -2.0,  0.0,  1.0,
              2.0,  0.0, -4.0,  0.0,  2.0,
              0.0,  0.0,  0.0,  0.0,  0.0,
             -2.0,  0.0,  4.0,  0.0, -2.0,
             -1.0,  0.0,  2.0,  0.0, -1.0
        ),

        new Kernel<5>( // R5E5
              -1.0,  -2.0,   0.0,   2.0,   1.0,
               4.0,   8.0,   0.0,  -8.0,  -4.0,
              -6.0, -12.0,   0.0,  12.0,   6.0,
               4.0,   8.0,   0.0,  -8.0,  -4.0,
              -1.0,  -2.0,   0.0,   2.0,   1.0
        ),

        new Kernel<5>( // R5R5
               1.0,  -4.0,   6.0,  -4.0,   1.0,
              -4.0,  16.0, -24.0,  16.0,  -4.0,
               6.0, -24.0,  36.0, -24.0,   6.0,
              -4.0,  16.0, -24.0,  16.0,  -4.0,
               1.0,  -4.0,   6.0,  -4.0,   1.0
        ),

        new Kernel<5>( // R5L5
               1.0,   4.0,   6.0,   4.0,   1.0,
              -4.0, -16.0, -24.0, -16.0,  -4.0,
               6.0,  24.0,  36.0,  24.0,   6.0,
              -4.0, -16.0, -24.0, -16.0,  -4.0,
               1.0,   4.0,   6.0,   4.0,   1.0
        ),

        new Kernel<5>( // R5S5
              -1.0,   0.0,   2.0,   0.0,  -1.0,
               4.0,   0.0,  -8.0,   0.0,   4.0,
              -6.0,   0.0,  12.0,   0.0,  -6.0,
               4.0,   0.0,  -8.0,   0.0,   4.0,
              -1.0,   0.0,   2.0,   0.0,  -1.0
        ),

        new Kernel<5>( // L5E5
              -1.0,  -2.0,   0.0,   2.0,   1.0,
              -4.0,  -8.0,   0.0,   8.0,   4.0,
              -6.0, -12.0,   0.0,  12.0,   6.0,
              -4.0,  -8.0,   0.0,   8.0,   4.0,
              -1.0,  -2.0,   0.0,   2.0,   1.0
        ),

        new Kernel<5>( // L5R5
               1.0,  -4.0,   6.0,  -4.0,   1.0,
               4.0, -16.0,  24.0, -16.0,   4.0,
               6.0, -24.0,  36.0, -24.0,   6.0,
               4.0, -16.0,  24.0, -16.0,   4.0,
               1.0,  -4.0,   6.0,  -4.0,   1.0
        ),

        new Kernel<5>( // L5L5
               1.0,   4.0,   6.0,   4.0,   1.0,
               4.0,  16.0,  24.0,  16.0,   4.0,
               6.0,  24.0,  36.0,  24.0,   6.0,
               4.0,  16.0,  24.0,  16.0,   4.0,
               1.0,   4.0,   6.0,   4.0,   1.0
        ),

        new Kernel<5>( // L5S5
              -1.0,   0.0,   2.0,   0.0,  -1.0,
              -4.0,   0.0,   8.0,   0.0,  -4.0,
              -6.0,   0.0,  12.0,   0.0,  -6.0,
              -4.0,   0.0,   8.0,   0.0,  -4.0,
              -1.0,   0.0,   2.0,   0.0,  -1.0
        ),

        new Kernel<5>( // S5E5
              1.0,  2.0,  0.0, -2.0, -1.0,
              0.0,  0.0,  0.0,  0.0,  0.0,
             -2.0, -4.0,  0.0,  4.0,  2.0,
              0.0,  0.0,  0.0,  0.0,  0.0,
              1.0,  2.0,  0.0, -2.0, -1.0
        ),

        new Kernel<5>( // S5R5
              -1.0,   4.0,  -6.0,   4.0,  -1.0,
               0.0,   0.0,   0.0,   0.0,   0.0,
               2.0,  -8.0,  12.0,  -8.0,   2.0,
               0.0,   0.0,   0.0,   0.0,   0.0,
              -1.0,   4.0,  -6.0,   4.0,  -1.0
        ),

        new Kernel<5>( // S5L5
              -1.0,  -4.0,  -6.0,  -4.0,  -1.0,
               0.0,   0.0,   0.0,   0.0,   0.0,
               2.0,   8.0,  12.0,   8.0,   2.0,
               0.0,   0.0,   0.0,   0.0,   0.0,
              -1.0,  -4.0,  -6.0,  -4.0,  -1.0
        ),

        new Kernel<5>( // S5S5
              1.0,  0.0, -2.0,  0.0,  1.0,
              0.0,  0.0,  0.0,  0.0,  0.0,
             -2.0,  0.0,  4.0,  0.0, -2.0,
              0.0,  0.0,  0.0,  0.0,  0.0,
              1.0,  0.0, -2.0,  0.0,  1.0
        ),

        NULL // End-of-array marking, do not remove
    };

    // Mask indices
    static const size_t E5E5 = 0;
    static const size_t E5R5 = 1;
    static const size_t E5L5 = 2;
    static const size_t E5S5 = 3;
    static const size_t R5E5 = 4;
    static const size_t R5R5 = 5;
    static const size_t R5L5 = 6;
    static const size_t R5S5 = 7;
    static const size_t L5E5 = 8;
    static const size_t L5R5 = 9;
    // static const size_t L5L5 = 10; // Ignored?
    static const size_t L5S5 = 11;
    static const size_t S5E5 = 12;
    static const size_t S5R5 = 13;
    static const size_t S5L5 = 14;
    static const size_t S5S5 = 15;

    List<cv::Mat> maps;
    cv::Mat values = normalize(data, w);
    for (const Kernel<5> **k = laws_masks; *k != NULL; k++) {
        const Kernel<5> &kernel = **k;
        cv::Mat e = energy(kernel(values), w);
        maps.append(e);
    }

    List<cv::Mat> output(9);
    output[0] = (maps[L5E5] + maps[E5L5]) / 2.0;
    output[1] = (maps[L5R5] + maps[R5L5]) / 2.0;
    output[2] = (maps[E5S5] + maps[S5E5]) / 2.0;
    output[3] = maps[S5S5];
    output[4] = maps[R5R5];
    output[5] = (maps[L5S5] + maps[S5L5]) / 2.0;
    output[6] = maps[E5E5];
    output[7] = (maps[E5R5] + maps[R5E5]) / 2.0;
    output[8] = (maps[S5R5] + maps[R5S5]) / 2.0;

    return output;
}

cv::Mat filter::normalize(const cv::Mat &data, size_t w) {
    CHANNEL_WISE(normalize, data, w);

    cv::Mat avg(data.size(), CV_64F);

    cv::Mat values;
    data.convertTo(values, CV_64F);

    size_t u = w / 2;
    for (size_t i = 0, m = data.rows; i < m; i++) {
        for (size_t j = 0, n = data.cols; j < n; j++) {
            size_t x = (j < u ? j : j - u);
            size_t y = (i < u ? i : i - u);
            size_t width = (x + w > n ? n - x : w);
            size_t height = (y + w > m ? m - y : w);

            cv::Rect roi(x, y, width, height);
            cv::Mat patch(values, roi);
            avg.at<float>(i, j) = values.at<float>(i, j) - cv::mean(patch)[0];
        }
    }

    return avg;
}

cv::Mat filter::otsu(cv::Mat image) {
    cv::threshold(image, image, 0, 1, cv::THRESH_BINARY + cv::THRESH_OTSU);
    return image;
}

List<cv::Mat> filter::prewitt(const cv::Mat &l) {
    static const Kernel<5>* prewitt_kernels[] = {
        new Kernel<5>(
            -1, -1,  0,  1,  1,
            -1, -1,  0,  1,  1,
            -1, -1,  0,  1,  1,
            -1, -1,  0,  1,  1,
            -1, -1,  0,  1,  1
        ),

        new Kernel<5>(
            -1,  0,  1,  1,  1,
            -1, -1,  1,  1,  1,
            -1, -1,  0,  1,  1,
            -1, -1, -1,  1,  1,
            -1, -1, -1,  0,  1
        ),

        new Kernel<5>(
             1,  1,  1,  1,  1,
             0,  1,  1,  1,  1,
            -1, -1,  0,  1,  1,
            -1, -1, -1, -1,  0,
            -1, -1, -1, -1, -1
        ),

        new Kernel<5>(
             1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,
             0,  0,  0,  0,  0,
            -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1
        ),

        new Kernel<5>(
             1,  1,  1,  1,  1,
             1,  1,  1,  1,  0,
             1,  1,  0, -1, -1,
             0, -1, -1, -1, -1,
            -1, -1, -1, -1, -1
        ),

        new Kernel<5>(
             1,  1,  1,  0, -1,
             1,  1,  1, -1, -1,
             1,  1,  0, -1, -1,
             1,  1, -1, -1, -1,
             1,  0, -1, -1, -1
        ),

        NULL // End-of-array marking, do not remove
    };

    List<cv::Mat> maps;
    for (const Kernel<5> **k = prewitt_kernels; *k != NULL; k++) {
        const Kernel<5> &kernel = **k;
        maps.append(kernel(l));
    }

    return maps;
}

cv::Mat filter::sobel(const cv::Mat &image) {
    List<cv::Mat> channels;
    cv::split(image, *channels);

    cv::Mat result = cv::Mat::zeros(image.size(), CV_8U);
    for (ListIterator<cv::Mat> i(channels); i.more();) {
        const cv::Mat &l = i.next();
        cv::Mat g, x, y, e;

        // Gradient X
        cv::Sobel(l, g, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(g, x);

        // Gradient Y
        cv::Sobel(l, g, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(g, y);

        // Total Gradient (approximate)
        cv::addWeighted(x, 0.5, y, 0.5, 0, g);
        cv::convertScaleAbs(g, e);
        e.copyTo(result, e);
    }

    return result;
}

cv::Mat filter::suppress(const cv::Mat &edges) {
    CHANNEL_WISE(suppress, edges);

    size_t rows = edges.rows;
    size_t cols = edges.cols;
    size_t i_l = rows - 1;
    size_t j_l = cols - 1;

    cv::Mat binary(edges.size(), CV_8U);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            bool i_0 = (i == 0);
            bool j_0 = (j == 0);
            bool i_n = (i == i_l);
            bool j_n = (j == j_l);

            uchar p_00 = (i_0 || j_0 ? 0 : edges.at<uchar>(i - 1, j - 1));
            uchar p_22 = (i_n || j_n ? 0 : edges.at<uchar>(i + 1, j + 1));

            uchar p_02 = (i_0 || j_n ? 0 : edges.at<uchar>(i - 1, j + 1));
            uchar p_20 = (i_n || j_0 ? 0 : edges.at<uchar>(i + 1, j - 1));

            uchar p_01 = (i_0 ? 0 : edges.at<uchar>(i - 1, j));
            uchar p_21 = (i_n ? 0 : edges.at<uchar>(i + 1, j));

            uchar p_10 = (j_0 ? 0 : edges.at<uchar>(i, j - 1));
            uchar p_12 = (j_n ? 0 : edges.at<uchar>(i, j + 1));

            uchar p_11 = edges.at<uchar>(i, j);

            bool on = (
                (p_00 < p_11 && p_22 < p_11) ||
                (p_02 < p_11 && p_20 < p_11) ||
                (p_01 < p_11 && p_21 < p_11) ||
                (p_10 < p_11 && p_12 < p_11)
            );

            binary.at<uchar>(i, j) = (on ? 255 : 0);
        }
    }

    return binary;
}

/*
Performs the preprocessing chain described in:

    Tan, X., and Triggs, B. "Enhanced local texture feature sets for face
    recognition under difficult lighting conditions.". IEEE Transactions
    on Image Processing 19 (2010), 1635–650.

Default parameters are taken from the paper. Original code taken from:

https://github.com/bytefish/opencv/blob/master/misc/tan_triggs.cpp
*/
cv::Mat filter::tantriggs(const cv::Mat &image, double a, double t, double g, double w0, double w1) {
    CHANNEL_WISE(tantriggs, image, a, t, g, w0, w1);

    // Convert to floating point
    cv::Mat output;
    image.convertTo(output, CV_64FC1);

    // Gamma correction
    output = gamma(output, g);

    // Difference of gaussians
    output = gaussian::difference(output, w0, w1);

    //return colors::discrete(output);

    // Constrast equalization parameters
    cv::Mat absd = cv::abs(output);
    cv::Mat mind = cv::min(absd, t);

    // Constrast equalization (first part)
    cv::Mat powd;
    cv::pow(absd, a, powd);
    double mean1 = cv::mean(powd)[0];
    output = output / pow(mean1, 1.0 / a);

    // Constrast equalization (second part)
    cv::pow(mind, a, powd);
    double mean2 = cv::mean(powd)[0];
    output = output / pow(mean2, 1.0 / a);

    // Constrast equalization (third part)
    for(int i = 0, m = output.rows; i < m; i++) {
        for(int j = 0, n = output.cols; j < n; j++) {
            output.at<double>(i, j) = t * tanh(output.at<double>(i, j) / t);
        }
    }

    return colors::discrete(output);
}
