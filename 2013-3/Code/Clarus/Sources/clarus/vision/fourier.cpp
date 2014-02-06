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

#include <clarus/vision/fourier.hpp>

cv::Size fourier::fit(const cv::Size &size) {
    return cv::Size(
        cv::getOptimalDFTSize(size.width),
        cv::getOptimalDFTSize(size.height)
    );
}

cv::Mat fourier::convolve(const cv::Mat &data, const cv::Mat &kernel) {
    cv::Size size = data.size();
    cv::Size optimal = fit(size);
    cv::Mat data_f = transform(data, optimal);
    cv::Mat kernel_f = transform(kernel, optimal);
    cv::Mat fourier = data_f.mul(-kernel_f);
    return inverse(fourier, size);
}

cv::Mat fourier::transform(const cv::Mat &data, const cv::Size &optimal) {
    cv::Size size = optimal;
    if (size.width == 0 || size.height == 0) {
        size = fit(data.size());
    }

    cv::Mat padded;
    int m = size.height - data.rows;
    int n = size.width - data.cols;
    cv::copyMakeBorder(data, padded, 0, m, 0, n, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    cv:: Mat plane[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};

    cv::Mat fourier;
    cv::merge(plane, 2, fourier);
    cv::dft(fourier, fourier, cv::DFT_COMPLEX_OUTPUT);

    // Cuts off negative frequencies, which are redundant for real inputs.
    return fourier(cv::Rect(0, 0, fourier.cols, 1 + fourier.rows / 2));
}

cv::Mat fourier::inverse(const cv::Mat &fourier, const cv::Size &optimal) {
    cv::Size size = optimal;
    if (size.width == 0 || size.height == 0) {
        size = fourier.size();
    }

    cv::Mat real;
    cv::dft(fourier, real, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

    return real(cv::Rect(0, 0, size.width, size.height));
}

cv::Mat fourier::magnitude(const cv::Mat &fourier) {
    std::vector<cv::Mat> plane;
    cv::split(fourier, plane);

    cv::Mat mag;
    cv::magnitude(plane[0], plane[1], mag);
    mag += cv::Scalar::all(1);
    cv::log(mag, mag);
    return mag;
}

cv::Mat fourier::phase(const cv::Mat &fourier) {
    std::vector<cv::Mat> plane;
    cv::split(fourier, plane);

    cv::Mat pha;
    cv::phase(plane[1], plane[0], pha);
    cv::normalize(pha, pha, 0, 127, CV_MINMAX);
    return pha;
}

cv::Mat fourier::bgr(const cv::Mat &fourier) {
    const cv::Size &size = fourier.size();
    cv::Mat hls(size, CV_8UC3);

    std::vector<cv::Mat> plane;
    cv::split(fourier, plane);

    cv::Mat mag;
    cv::magnitude(plane[0], plane[1], mag);
    mag += cv::Scalar::all(1);
    cv::log(mag, mag);
    cv::normalize(mag, mag, 127, 255, CV_MINMAX);

    cv::Mat pha;
    cv::phase(plane[1], plane[0], pha);
    cv::normalize(pha, pha, 0, 127, CV_MINMAX);

    for (int i = 0, m = size.height; i < m; i++) {
        for (int j = 0, n = size.width; j < n; j++) {
            cv::Vec3b &pixel = hls.at<cv::Vec3b>(i, j);
            pixel[0] = (uchar) pha.at<float>(i, j);
            pixel[1] = (uchar) mag.at<float>(i, j);
            pixel[2] = 255;
        }
    }

    cv::Mat rgb;
    cv::cvtColor(hls, rgb, CV_HLS2BGR);
    return rgb;
}
