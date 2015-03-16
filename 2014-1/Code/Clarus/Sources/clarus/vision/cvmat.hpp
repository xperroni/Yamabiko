#ifndef CLARUS_VISION_CVMAT_HPP
#define CLARUS_VISION_CVMAT_HPP

#include <opencv2/opencv.hpp>

#include <cstdarg>
#include <string>

namespace cvmat {
    namespace type {
        int channels(int type);

        uchar depth(int type);

        int length(int type);

        std::string name(int type);
    }

    /*
    Returns the address to a cell in a multidimensional matrix.

    Reference: http://docs.opencv.org/2.4.5/modules/core/doc/basic_structures.html#mat

    Arguments:

    src
        Source matrix.

    ...
        Sequence of int indices to the desired cell.

    Returns:

    The address to the desired cell.
    */
    uchar *address(cv::Mat &src, ...);

    /*
    Vararg version of address().
    */
    uchar *address(cv::Mat &src, va_list &args);

    /*
    Const version of address().
    */
    const uchar *address(const cv::Mat &src, ...);

    /*
    Const version of vararg address().
    */
    const uchar *address(const cv::Mat &src, va_list &args);

    /*
    Converts the given matrix to the given data type.
    */
    cv::Mat convert(const cv::Mat &image, int type);

    /*
    Copies a value of the given type between two cell addresses.


    Arguments:

    type
        Type code of the cells, as returned by cv::Mat::type().

    dst
        Destination cell.

    src
        Source cell.
    */
    void copy(int type, uchar *dst, const uchar *src);

    /*
    Performs a type-independent copy of a value from a source matrix to a destination
    matrix. The only restriction is that both matrices must be of the same primitive
    type -- i.e. one of the pre-defined CV_*(U|S)C* types (e.g. CV_32SC1, CV_8UC3, etc).

    Arguments:

    dst
        Destination matrix.

    i0, j0
        Row and column index of the destination cell.

    src
        Source matrix.

    i1, j1
        Row and column index of the destination cell.
    */
    void copy(cv::Mat &dst, int i0, int j0, const cv::Mat &src, int i1, int j1);

    /*
    Returns a vector containing the value of the addressed cell in the source matrix.

    Arguments:

    src
        Source matrix.

    ...
        Sequence of int indices to the desired cell.

    Returns:

    A vector containing the value of the addressed cell. The output vector will have
    the same dimension as the source matrix channel count (i.e. 1 for single-channel
    matrices, 3 for 3-channel, etc).
    */
    cv::Mat retrieve(const cv::Mat &src, ...);

    /*
    Vararg version of retrieve().
    */
    cv::Mat retrieve(const cv::Mat &src, va_list &args);

    /*
    Returns a scalar containing the value of the addressed cell in the source matrix.

    Arguments:

    src
        Source matrix.

    ...
        Sequence of int indices to the desired cell.

    Returns:

    A scalar containing the value(s) of the addressed cell. If the matrix has less than
    four channels, the remaining higher positions are set to zero; if it has more, only
    the first four lowest-level channel values are returned.
    */
    cv::Scalar scalar(const cv::Mat &src, ...);

    /*
    Vararg version of scalar().
    */
    cv::Scalar scalar(const cv::Mat &src, va_list &args);

    /*
    Returns whether the given matrix cell is zero.
    */
    bool zero(const cv::Mat &src, int i, int j);
}

#endif
