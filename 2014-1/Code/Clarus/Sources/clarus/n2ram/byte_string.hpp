#ifndef CLARUS_N2RAM_BYTE_STRING_HPP
#define CLARUS_N2RAM_BYTE_STRING_HPP

#include <boost/shared_array.hpp>

#include <opencv2/opencv.hpp>

namespace n2ram {
    struct ByteString;
}

struct n2ram::ByteString {
    boost::shared_array<uint8_t> buffer;

    /*
    Creates a new byte string of given size, with all bytes set to 0.
    */
    ByteString(size_t n);

    /*
    Creates a new byte string object from the given matrix cell. No data is copied,
    so this object is only valid as long as the parent matrix exists.
    */
    ByteString(const cv::Mat &data, size_t i, size_t j);

    /*
    Creates a new byte string from a slice of an existing one, staring at the given offset
    all the way to the string's end. If flyweight is true, the new object only points to
    a position in the parent's buffer, otherwise a new buffer is created and owned by the
    new object.
    */
    ByteString(const ByteString &that, size_t offset, bool flyweight);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ByteString();

    /*
    Returns the byte at the given position.
    */
    uint8_t operator [] (const size_t index) const;

    /*
    Calculates the hamming distance between this byte string and another one, from a
    given offset to the end of the shortest buffer.
    */
    int distance(const ByteString &that, size_t offset) const;

    /*
    Returns the size of this byte string.
    */
    size_t size() const;

    /*
    Resizes this byte string. Currently this can only be used to shrink the string;
    attempts to enlarge it will result in an exception.
    */
    void resize(size_t n);

private:
    size_t bytes;
};

#endif // CLARUS_N2RAM_BYTE_STRING_HPP
