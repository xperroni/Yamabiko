#include <clarus/vision/features.hpp>
using clarus::List;

#include <clarus/vgram/bitstring.hpp>
using vgram::Bitstring;

#include <cstring>

cv::Mat features::LBP(const cv::Mat &image) {
    int rows = image.rows;
    int cols = image.cols;
    cv::Mat patterns(rows, cols, CV_8U);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uint8_t p = 0;
            uint8_t c = image.at<uint8_t>(i, j);
            for (uint8_t s = 0; s < 2; s++) {
                for(uint8_t k = 5 * s, n = k + 4; k < n; k++) {
                    int i2 = i - 1 + k / 3;
                    int j2 = j - 1 + k % 3;
                    if (i2 < 0 || 12 >= rows || j2 < 0 || j2 >= cols) {
                        continue;
                    }

                    if (c < image.at<uint8_t>(i2, j2)) {
                        p |= (0x01 << (k - s));
                    }
                }
            }

            patterns.at<uint8_t>(i, j) = p;
        }
    }

    return patterns;
}

inline void desaturate(cv::Mat &image) {
    static cv::Mat noise;
    if (noise.size() != image.size()) {
        noise = image.clone();
        cv::randn(noise, 10, 10);
    }

    cv::add(image, noise, image, image == 0);
    cv::subtract(image, noise, image, image == 255);
}

cv::Mat features::FREAK(const cv::Mat &image, float size, int step, int bytes) {
    int border = size * 4;

    // Adds a border around the image.
    cv::Mat canvas;
    cv::copyMakeBorder(
        image, canvas,
        border, border, border, border,
        cv::BORDER_CONSTANT, cv::Scalar::all(0)
    );

    // Adds random noise to saturated image regions.
    //desaturate(canvas);

    List<cv::KeyPoint> keypoints;
    cv::DenseFeatureDetector detector(size, 1, 1.0f, step, border);
    detector.detect(canvas, *keypoints);

    cv::Mat descriptors;
    cv::FREAK extractor;
    extractor.compute(canvas, *keypoints, descriptors);

    int rows = image.rows / step;
    int cols = image.cols / step;

    const int sizes[] = {rows, cols, bytes};
    cv::Mat output(3, sizes, CV_8U, cv::Scalar::all(0));

    for (size_t k = 0, n = keypoints.size(); k < n; k++) {
        uint8_t *features = descriptors.ptr<uint8_t>(k);
        const cv::KeyPoint &keypoint = keypoints[k];
        int i = (keypoint.pt.y - border) / step;
        int j = (keypoint.pt.x - border) / step;

        if (i >= rows || j >= cols) {
            continue;
        }

        uint8_t *bitstring = &(output.at<uint8_t>(i, j, 0));
        memcpy(bitstring, features, bytes);
    }

    return output;
}

List<cv::Mat> features::splitFREAK(const cv::Mat &image, float size, int step) {
    cv::Mat descriptors = FREAK(image, size, step);

    size_t rows = image.rows / step;
    size_t cols = image.cols / step;

    List<cv::Mat> split;
    for (size_t l = 0; l < 4; l++) {
        cv::Mat level(rows, cols, CV_32SC4, cv::Scalar::all(0));
        split.append(level);
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            uint8_t *features = &(descriptors.at<uint8_t>(i, j, 0));
            for (size_t l = 0; l < 4; l++) {
                uint8_t *bitstring = (uint8_t*) &(split[l].at<cv::Vec4i>(i, j)[0]);
                memcpy(bitstring, features + 16 * l, 16);
            }
        }
    }

    return split;
}

cv::Mat features::minchinton(const cv::Mat &image, int side) {
    int rows = image.rows / side;
    int cols = image.cols / side;
    int vals = side * side / 8;

    const int sizes[] = {rows, cols, vals};

    cv::Mat descriptor(3, sizes, CV_8U);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Bitstring bits(descriptor, i, j);
            cv::Rect roi(j * side, i * side, side, side);
            cv::Mat patch(image, roi);
            size_t size = patch.total();
            size_t last = size - 1;
            for (size_t k = 0; k < last; k++) {
                uint8_t a = patch.at<uint8_t>(k);
                uint8_t b = patch.at<uint8_t>(k + 1);
                bits.set(k, a > b);
            }

            bits.set(last, patch.at<uint8_t>(last) > patch.at<uint8_t>(0));
        }
    }

    return descriptor;
}
