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

#include <clarus/vision/segment.hpp>

#include <clarus/core/list.hpp>
using clarus::List;
using clarus::ListIteratorConst;

#include <algorithm>

inline void blob_set(
    List<int> &neighbors,
    float value,
    const cv::Mat &data,
    const cv::Mat &labels,
    int i, int j,
    float threshold
) {
    if (i < 0 || j < 0 || i >= data.rows || j >= data.cols) {
        return;
    }

    if (fabs(data.at<float>(i, j) - value) < threshold) {
        neighbors.append(labels.at<float>(i, j));
    }
}

inline List<int> blob_neighbors(
    const cv::Mat &data,
    const cv::Mat &labels,
    int i, int j,
    float threshold
) {
    List<int> neighbors;

    float value = data.at<float>(i, j);
    blob_set(neighbors, value, data, labels, i - 1, j - 1, threshold);
    blob_set(neighbors, value, data, labels, i - 1, j + 0, threshold);
    blob_set(neighbors, value, data, labels, i - 1, j + 1, threshold);
    blob_set(neighbors, value, data, labels, i + 0, j - 1, threshold);

    std::sort(neighbors->begin(), neighbors->end());

    return neighbors;
}

cv::Mat segment::blob(const cv::Mat &data, float threshold) {
    int rows = data.rows;
    int cols = data.cols;
    cv::Mat labels(rows, cols, CV_64F, cv::Scalar(0));
    std::map<int, int> linked;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            List<int> neighbors = blob_neighbors(data, labels, i, j, threshold);
            if (neighbors.size() == 0) {
                int label = linked.size();
                linked[label] = label;
                labels.at<float>(i, j) = label;
            }
            else {
                int label = neighbors[0];
                labels.at<float>(i, j) = label;
                for (ListIteratorConst<int> i(neighbors); i.more();) {
                    linked[i.next()] = label;
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int label = labels.at<float>(i, j);
            labels.at<float>(i, j) = linked[label];
        }
    }

    return labels;
}

cv::Mat segment::kmeans(const cv::Mat &image, int cluster_number) {
    cv::Mat reshaped_image = image.reshape(1, image.cols * image.rows);

    cv::Mat reshaped_image32f;
    reshaped_image.convertTo(reshaped_image32f, CV_64FC1, 1.0 / 255.0);

    cv::Mat labels;
    cv::TermCriteria criteria(cv::TermCriteria::COUNT, 100, 1);
    cv::Mat centers;
    cv::kmeans(reshaped_image32f, cluster_number, labels, criteria, 1, cv::KMEANS_RANDOM_CENTERS, centers);

    int width = image.cols;
    int height = image.rows;

    cv::Mat rgb_image(height, width, CV_8UC3);
    cv::MatIterator_<cv::Vec3b> rgb_first = rgb_image.begin<cv::Vec3b>();
    cv::MatIterator_<cv::Vec3b> rgb_last = rgb_image.end<cv::Vec3b>();
    cv::MatConstIterator_<int> label_first = labels.begin<int>();

    cv::Mat centers_u8;
    centers.convertTo(centers_u8, CV_8UC1, 255.0);
    cv::Mat centers_u8c3 = centers_u8.reshape(3);

    while ( rgb_first != rgb_last ) {
        const cv::Vec3b& rgb = centers_u8c3.ptr<cv::Vec3b>(*label_first)[0];
        *rgb_first = rgb;
        ++rgb_first;
        ++label_first;
    }

    return rgb_image;
}
