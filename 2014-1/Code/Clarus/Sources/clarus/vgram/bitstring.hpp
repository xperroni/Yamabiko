#ifndef CLARUS_VGRAM_BITSTRING_HPP
#define CLARUS_VGRAM_BITSTRING_HPP

#include <boost/shared_array.hpp>

#include <opencv2/opencv.hpp>

namespace vgram {
    struct Bitstring;
}

struct vgram::Bitstring {
    boost::shared_array<uint8_t> buffer;

    uint32_t n;

    /*
    Creates a new bot string object from the given matrix cell.
    */
    Bitstring(const cv::Mat &data, size_t i, size_t j);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Bitstring();

    void set(uint32_t k, bool on);

    uint32_t distance(const Bitstring &that) const;

    /**
    \brief Returns the size of bit strings stored in the given matrix, in bytes.
    */
    static uint32_t bytes(const cv::Mat &data);

    static cv::Point3i closest(const cv::Mat &m1, int i, int j, const cv::Mat &m2);

    static cv::Point3i closest(const cv::Mat &m1, int i, int j);

    static cv::Point3i closest(const uint8_t *bitstring, const cv::Mat &compared);

    static uint32_t distance(const uint8_t *a, const uint8_t *b, uint32_t n);

    static cv::Mat distance(const cv::Mat &m1, const cv::Mat &m2);

    static uint32_t distance(const cv::Mat &m1, const cv::Mat &m2, int i, int j);

    static uint32_t distance(const cv::Mat &m1, const cv::Mat &m2, const cv::Rect &rect);

    static uint8_t *pointer(const cv::Mat &data, size_t i, size_t j);

    uint32_t weight() const;

    static uint32_t weight(const uint8_t *bitstring, uint32_t length);
};

#endif // CLARUS_VGRAM_BITSTRING_HPP
