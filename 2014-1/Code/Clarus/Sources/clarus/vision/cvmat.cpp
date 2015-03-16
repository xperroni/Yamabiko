#include <clarus/vision/cvmat.hpp>

#include <clarus/core/domain.hpp>

#include <stdexcept>

int cvmat::type::channels(int type) {
    return 1 + (type >> CV_CN_SHIFT);
}

uchar cvmat::type::depth(int type) {
    return type & CV_MAT_DEPTH_MASK;
}

int cvmat::type::length(int type) {
    int chans = type::channels(type);
    uchar depth = type::depth(type);

    switch (depth) {
        case CV_8U  : return chans;
        case CV_8S  : return chans;
        case CV_16U : return chans * 2;
        case CV_16S : return chans * 2;
        case CV_32S : return chans * 4;
        case CV_32F : return chans * 4;
        case CV_64F : return chans * 8;
        default: throw std::runtime_error("Cannot identify non-primitive type");
    }
}

std::string cvmat::type::name(int type) {
    std::string name;
    uchar depth = type::depth(type);
    switch (depth) {
        case CV_8U  : name = "8U";   break;
        case CV_8S  : name = "8S";   break;
        case CV_16U : name = "16U";  break;
        case CV_16S : name = "16S";  break;
        case CV_32S : name = "32S";  break;
        case CV_32F : name = "32F";  break;
        case CV_64F : name = "64F";  break;
        default     : name = "USER"; break;
    }

    int chans = type::channels(type);
    return name + "C" + domain::to_string(chans);
}

uchar *cvmat::address(cv::Mat &src, va_list &args) {
    int dims = src.dims;
    uchar *data = src.data;
    const cv::Mat::MStep &step = src.step;

    for (int i = 0; i < dims; i++) {
        int index = va_arg(args, int);
        data += step[i] * index;
    }

    return data;
}

const uchar *cvmat::address(const cv::Mat &src, va_list &args) {
    int dims = src.dims;
    const uchar *data = src.data;
    const cv::Mat::MStep &step = src.step;

    for (int i = 0; i < dims; i++) {
        int index = va_arg(args, int);
        data += step[i] * index;
    }

    return data;
}

cv::Mat cvmat::convert(const cv::Mat &image, int type) {
    cv::Mat converted;
    image.convertTo(converted, type);
    return converted;
}

cv::Mat cvmat::retrieve(const cv::Mat &src, va_list &args) {
    const uchar *cell = address(src, args);
    int type = src.type();
    cv::Mat data(1, type::channels(type), type::depth(type));
    memcpy(data.ptr<uchar>(0), cell, type::length(type));
    return data;
}

inline double retrieve(int channel, int type, const uchar *data) {
    uchar depth = cvmat::type::depth(type);
    switch (depth) {
        case CV_8S  : return *(((int8_t*) data) + channel);
        case CV_16S : return *(((int16_t*) data) + channel);
        case CV_8U  : return *(((uint8_t*) data) + channel);
        case CV_16U : return *(((uint16_t*) data) + channel);
        case CV_32S : return *(((int32_t*) data) + channel);
        case CV_32F : return *(((float*) data) + channel);
        case CV_64F : return *(((double*) data) + channel);
    }

    throw std::runtime_error("Non-primitive data type");
}

cv::Scalar cvmat::scalar(const cv::Mat &src, va_list &args) {
    int type = src.type();
    const uchar *data = address(src, args);

    cv::Scalar value = cv::Scalar::all(0);
    for (int i = 0, n = std::min(cvmat::type::channels(type), 4); i < n; i++) {
        value[i] = ::retrieve(i, type, data);
    }

    return value;
}

uchar *cvmat::address(cv::Mat &src, ...) {
    va_list args;
    va_start(args, src);
    uchar *data = address(src, args);
    va_end(args);
    return data;
}

const uchar *cvmat::address(const cv::Mat &src, ...) {
    va_list args;
    va_start(args, src);
    const uchar *data = address(src, args);
    va_end(args);
    return data;
}

cv::Mat cvmat::retrieve(const cv::Mat &src, ...) {
    va_list args;
    va_start(args, src);
    cv::Mat data = retrieve(src, args);
    va_end(args);
    return data;
}

cv::Scalar cvmat::scalar(const cv::Mat &src, ...) {
    va_list args;
    va_start(args, src);
    cv::Scalar value = scalar(src, args);
    va_end(args);
    return value;
}

void cvmat::copy(int type, uchar *dst, const uchar *src) {
    memcpy(dst, src, type::length(type));
}

void cvmat::copy(cv::Mat &dst, int i0, int j0, const cv::Mat &src, int i1, int j1) {
    int dt = dst.type();
    int st = src.type();
    if (dt != st) {
        throw std::runtime_error(
            "Types of source (" + type::name(st) + ") and destination (" + type::name(dt) + ") differ"
        );
    }

    copy(dt, address(dst, i0, j0), address(src, i1, j1));
}

bool cvmat::zero(const cv::Mat &src, int i, int j) {
    for (const uchar *b = address(src, i, j), *n = b + type::length(src.type()); b < n; b++) {
        if (*b != 0) {
            return false;
        }
    }

    return true;
}
