#include <clarus/n2ram/byte_string.hpp>
using n2ram::ByteString;

#include <clarus/vision/cvmat.hpp>

#define NDEBUG
#include <cassert>

#include <cstring>
#include <stdexcept>

ByteString::ByteString(size_t n):
    buffer(),
    bytes(n)
{
    if (n > 0) {
        buffer = boost::shared_array<uint8_t>(new uint8_t[n]);
        memset(buffer.get(), 0x00, n);
    }
}

// Dummy buffer destructor used when a ByteString object is working as a Flyweight
// to a buffer managed by another object (e.g. an OpenCV Mat).
static void do_nothing(uint8_t *array) {
    // Nothing to do.
}

ByteString::ByteString(const ByteString &that, size_t offset, bool flyweight):
    buffer(),
    bytes(that.bytes - offset)
{
    uint8_t *slice = that.buffer.get() + offset;
    if (flyweight) {
        buffer = boost::shared_array<uint8_t>(slice, do_nothing);
    }
    else {
        uint8_t *copied = new uint8_t[bytes];
        memcpy(copied, slice, bytes);
        buffer = boost::shared_array<uint8_t>(copied);
    }

    assert(bytes <= that.bytes);
}

template<class T> inline uint8_t *scalarBuffer(const cv::Mat &data, size_t i, size_t j) {
    return (uint8_t*) &(data.dims == 2 ? data.at<T>(i, j) : data.at<T>(i, j, 0));
}

template<class T> inline uint8_t *vectorBuffer(const cv::Mat &data, size_t i, size_t j) {
    return (uint8_t*) &((data.dims == 2 ? data.at<T>(i, j) : data.at<T>(i, j, 0))[0]);
}

inline uint8_t *get_buffer(const cv::Mat &data, size_t i, size_t j) {
    int dtype = data.type();
    switch (dtype) {
        case CV_8U    : return scalarBuffer<uint8_t>(data, i, j);
        case CV_32S   : return scalarBuffer<uint32_t>(data, i, j);
        case CV_32SC4 : return vectorBuffer<cv::Vec4i>(data, i, j);
        default:
            throw std::runtime_error("Unsupported type " + cvmat::type::name(dtype));
    }

    return NULL;
}

inline size_t get_length(const cv::Mat &data) {
    return (data.dims > 2 ? data.size[2] : 1) * cvmat::type::length(data.type());
}

ByteString::ByteString(const cv::Mat &data, size_t i, size_t j):
    buffer(get_buffer(data, i, j), do_nothing),
    bytes(get_length(data))
{
    // Nothing to do.
}

ByteString::~ByteString() {
    // Nothing to do.
}

uint8_t ByteString::operator [] (const size_t index) const {
    return buffer[index];
}

template<class T> inline uint32_t distance_primitive(T a, T b) {
    return __builtin_popcount(a ^ b);
}

template<class T> inline uint32_t distance_buffer(const T *a, const T *b, uint32_t n) {
    uint32_t d = 0;
    for (uint32_t i = 0; i < n; i++) {
        d += __builtin_popcount(a[i] ^ b[i]);
    }

    return d;
}

inline uint32_t distance_128(uint64_t *a, uint64_t *b) {
    return
        __builtin_popcount(a[0] ^ b[0]) +
        __builtin_popcount(a[1] ^ b[1]);
}

inline uint32_t distance_256(uint64_t *a, uint64_t *b) {
    return
        distance_128(a, b) +
        distance_128(a + 2, b + 2);
}

inline uint32_t distance_512(uint64_t *a, uint64_t *b) {
    return
        distance_256(a, b) +
        distance_256(a + 4, b + 4);
}

inline uint32_t distance_n(const uint8_t *a, const uint8_t *b, uint32_t n) {
    if (n % 8 == 0) {
        return distance_buffer<uint64_t>((uint64_t*) a, (uint64_t*) b, n / 8);
    }

    if (n % 4 == 0) {
        return distance_buffer<uint32_t>((uint32_t*) a, (uint32_t*) b, n / 4);
    }

    if (n % 2 == 0) {
        return distance_buffer<uint16_t>((uint16_t*) a, (uint16_t*) b, n / 2);
    }

    return distance_buffer<uint8_t>(a, b, n);
}

inline int hamming_distance(uint8_t *a, uint8_t *b, size_t n) {
    switch (n) {
        case 1  : return distance_primitive<uint8_t>(*a, *b);
        case 2  : return distance_primitive<uint16_t>(*((uint16_t*) a), *((uint16_t*) b));
        case 4  : return distance_primitive<uint32_t>(*((uint32_t*) a), *((uint32_t*) b));
        case 8  : return distance_primitive<uint64_t>(*((uint64_t*) a), *((uint64_t*) b));
        case 16 : return distance_128((uint64_t*) a, (uint64_t*) b);
        case 32 : return distance_256((uint64_t*) a, (uint64_t*) b);
        case 64 : return distance_512((uint64_t*) a, (uint64_t*) b);
        default : return distance_n(a, b, n);
    }
}

int ByteString::distance(const ByteString &that, size_t offset) const {
    ByteString slice(that, offset, true);
    size_t n = std::min(bytes, slice.bytes);
    return hamming_distance(buffer.get(), that.buffer.get(), n);
}

size_t ByteString::size() const {
    return bytes;
}


void ByteString::resize(size_t n) {
    if (n > bytes) {
        throw std::runtime_error("Byte string size cannot be increased");
    }

    bytes = n;
}
