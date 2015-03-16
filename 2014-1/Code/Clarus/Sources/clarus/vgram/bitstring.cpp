#include <clarus/vgram/bitstring.hpp>
using vgram::Bitstring;

#include <clarus/vision/cvmat.hpp>

#include <stdexcept>

// Dummy buffer destructor used when a Bitstring object is working as a Flyweight
// to a buffer managed by another object (e.g. an OpenCV Mat).
static void do_nothing(uint8_t *array) {
    // Nothing to do.
}

Bitstring::Bitstring(const cv::Mat &data, size_t i, size_t j):
    buffer(pointer(data, i, j), do_nothing),
    n(bytes(data))
{
    // Nothing to do.
}

Bitstring::~Bitstring() {
    // Nothing to do.
}

template<class T> inline uint32_t distance_(T a, T b) {
    return __builtin_popcount(a ^ b);
}

#ifdef __SSE4_2__
#include <smmintrin.h>
#endif

template<> inline uint32_t distance_(uint64_t a, uint64_t b) {
#ifdef _mm_popcnt_u64
    return _mm_popcnt_u64(a ^ b);
#else
    return __builtin_popcountll(a ^ b);
#endif
}

//Reference: http://bmagic.sourceforge.net/bmsse2opt.html
inline uint32_t distance_128(uint64_t *p1, uint64_t *p2) {
#ifdef __SSE4_1__
    const uint32_t mu1 = 0x55555555;
    const uint32_t mu2 = 0x33333333;
    const uint32_t mu3 = 0x0f0f0f0f;
    const uint32_t mu4 = 0x0000003f;

    __m128i m1 = _mm_set_epi32(mu1, mu1, mu1, mu1);
    __m128i m2 = _mm_set_epi32(mu2, mu2, mu2, mu2);
    __m128i m3 = _mm_set_epi32(mu3, mu3, mu3, mu3);
    __m128i m4 = _mm_set_epi32(mu4, mu4, mu4, mu4);

    __m128i v1 = _mm_load_si128((__m128i*) p1);
    __m128i v2 = _mm_load_si128((__m128i*) p2);
    __m128i b = _mm_xor_si128(v1, v2);

    __m128i tmp1, tmp2;

    tmp1 = _mm_srli_epi32(b, 1);
    tmp1 = _mm_and_si128(tmp1, m1);
    tmp2 = _mm_and_si128(b, m1);
    b = _mm_add_epi32(tmp1, tmp2);

    tmp1 = _mm_srli_epi32(b, 2);
    tmp1 = _mm_and_si128(tmp1, m2);
    tmp2 = _mm_and_si128(b, m2);
    b = _mm_add_epi32(tmp1, tmp2);

    tmp1 = _mm_srli_epi32(b, 4);
    b = _mm_add_epi32(b, tmp1);
    b = _mm_and_si128(b, m3);

    tmp1 = _mm_srli_epi32(b, 8);
    b = _mm_add_epi32(b, tmp1);

    tmp1 = _mm_srli_epi32(b, 16);
    b = _mm_add_epi32(b, tmp1);
    b = _mm_and_si128(b, m4);

    uint32_t counts[4];
    _mm_storeu_si128((__m128i*) counts, b);
    return counts[0] + counts[1] + counts[2] + counts[3];
#else
    return
        distance_<uint64_t>(a[0], b[0]) +
        distance_<uint64_t>(a[1], b[1]);
#endif
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

template<class T> inline uint32_t distance_buffer(const T *a, const T *b, uint32_t n) {
    uint32_t d = 0;
    for (uint32_t i = 0; i < n; i++) {
        d += distance_<T>(a[i], b[i]);
    }

    return d;
}

inline uint32_t distance_n(const uint8_t *a, const uint8_t *b, uint32_t n) {
    uint32_t d = 0;
    while (n > 0) {
        int r = 0;
        if (n > 8) {
            d += distance_buffer<uint64_t>((uint64_t*) a, (uint64_t*) b, n / 8);
            r = n % 8;
        }
        else if (n > 4) {
            d += distance_buffer<uint32_t>((uint32_t*) a, (uint32_t*) b, n / 4);
            r = n % 4;
        }
        else if (n > 2) {
            d += distance_buffer<uint16_t>((uint16_t*) a, (uint16_t*) b, n / 2);
            r = n % 2;
        }
        else {
            d += distance_buffer<uint8_t>(a, b, n);
            break;
        }

        int s = n - r;
        a += s;
        b += s;
        n = r;
    }

    return d;
}

uint32_t Bitstring::bytes(const cv::Mat &data) {
    return (data.dims > 2 ? data.size[2] : 1) * cvmat::type::length(data.type());
}

uint32_t Bitstring::distance(const Bitstring &that) const {
    return distance(this->buffer.get(), that.buffer.get(), n);
}

cv::Point3i Bitstring::closest(const cv::Mat &m1, int i, int j, const cv::Mat &m2) {
    return closest(pointer(m1, i, j), m2);
}

cv::Point3i Bitstring::closest(const cv::Mat &m1, int i, int j) {
    const uint8_t *bitstring = pointer(m1, i, j);
    uint32_t least = std::numeric_limits<uint32_t>::max();
    uint32_t n = bytes(m1);
    int rows = m1.size[0];
    int cols = m1.size[1];

    cv::Point3i best(0, 0, least);
    if (weight(bitstring, n) == 0) {
        return best;
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (y == i && x == j) {
                continue;
            }

            const uint8_t *other = pointer(m1, y, x);
            if (weight(other, n) == 0) {
                continue;
            }

            uint32_t d = Bitstring::distance(bitstring, other, n);
            if (d < least) {
                least = d;
                best.x = x;
                best.y = y;
                best.z = d;
            }
        }
    }

    return best;
}

cv::Point3i Bitstring::closest(const uint8_t *bitstring, const cv::Mat &compared) {
    uint32_t least = std::numeric_limits<uint32_t>::max();
    uint32_t n = bytes(compared);
    uint8_t *data = compared.data;
    int rows = compared.size[0];
    int cols = compared.size[1];

    cv::Point3i best;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++, data += n) {
            uint32_t d = Bitstring::distance(bitstring, data, n);
            if (d < least) {
                least = d;
                best.x = x;
                best.y = y;
                best.z = d;
            }
        }
    }

    return best;
}

uint32_t Bitstring::distance(const uint8_t *a, const uint8_t *b, uint32_t n) {
    switch (n) {
        case 1  : return distance_<uint8_t>(*a, *b);
        case 2  : return distance_<uint16_t>(*((uint16_t*) a), *((uint16_t*) b));
        case 4  : return distance_<uint32_t>(*((uint32_t*) a), *((uint32_t*) b));
        case 8  : return distance_<uint64_t>(*((uint64_t*) a), *((uint64_t*) b));
        case 16 : return distance_128((uint64_t*) a, (uint64_t*) b);
        case 32 : return distance_256((uint64_t*) a, (uint64_t*) b);
        case 64 : return distance_512((uint64_t*) a, (uint64_t*) b);
        default : return distance_n(a, b, n);
    }
}

cv::Mat Bitstring::distance(const cv::Mat &m1, const cv::Mat &m2) {
    int rows = m1.size[0];
    int cols = m1.size[1];
    cv::Mat d(rows, cols, CV_32S, cv::Scalar(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            d.at<int>(i, j) = distance(m1, m2, i, j);
        }
    }

    return d;
}

uint32_t Bitstring::distance(const cv::Mat &m1, const cv::Mat &m2, int i, int j) {
    int n = bytes(m1);
    uint8_t *b1 = pointer(m1, i, j);
    uint8_t *b2 = pointer(m2, i, j);
    return distance(b1, b2, n);
}

uint32_t Bitstring::distance(const cv::Mat &m1, const cv::Mat &m2, const cv::Rect &rect) {
    int row0 = rect.y;
    int col0 = rect.x;
    int rows = row0 + rect.height;
    int cols = col0 + rect.width;
    int n = bytes(m1);

    uint32_t total = 0;
    for (int i = row0; i < rows; i++) {
        for (int j = col0; j < cols; j++) {
            uint8_t *b1 = pointer(m1, i, j);
            uint8_t *b2 = pointer(m2, i, j);
            total += distance(b1, b2, n);
        }
    }

    return total;
}

template<class T> inline uint8_t *scalarBuffer(const cv::Mat &data, size_t i, size_t j) {
    return (uint8_t*) &(data.dims == 2 ? data.at<T>(i, j) : data.at<T>(i, j, 0));
}

template<class T> inline uint8_t *vectorBuffer(const cv::Mat &data, size_t i, size_t j) {
    return (uint8_t*) &((data.dims == 2 ? data.at<T>(i, j) : data.at<T>(i, j, 0))[0]);
}

uint8_t *Bitstring::pointer(const cv::Mat &data, size_t i, size_t j) {
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

uint32_t Bitstring::weight() const {
    return weight(this->buffer.get(), n);
}

uint32_t Bitstring::weight(const uint8_t *bitstring, uint32_t length) {
    uint32_t w = 0;
    for (uint32_t i = 0; i < length; i++) {
        w += __builtin_popcount(bitstring[i]);
    }

    return w;
}

void Bitstring::set(uint32_t k, bool on) {
    uint32_t i = k / 8;
    uint8_t j = k % 8;

    if (on) {
        buffer[i] |= (0x80 >> j);
    }
    else {
        buffer[i] &= ~(0x80 >> j);
    }
}
