#ifndef CLARUS_VGRAM_CONTROL_HPP
#define CLARUS_VGRAM_CONTROL_HPP

#include <clarus/core/list.hpp>

#include <boost/shared_ptr.hpp>

#include <opencv2/opencv.hpp>

namespace vgram {
    struct Control;
}

/**
Basic interface for all VG-RAM control objects.

VG-RAM control objects are used to change the way retrieve operations access layer
memory in one way or another. The typical use case is to restrict access to records
that are deemed more likely to contain good output estimates for a given input,
reducing search times and output noise.
*/
struct vgram::Control {
    /** \brief Smart Control pointer type. */
    typedef boost::shared_ptr<Control> P;

    /**
    \brief Extracts a bit string from the given input matrix.

    The input matrix is expected to be of type \c CV_8U. Its layout can be either 2D
    or 3D. The returned value is a pointer to the beginning of a cell in the matrix,
    therefore the matrix must be guaranteed to exist for as long as the pointer is
    in use. Which cell is selected depends on the interface's implementation.
    */
    virtual uint8_t *bitstring(const cv::Mat &input) = 0;

    /**
    \brief Extracts a bit string from the given list of known inputs.

    Input matrices in the list are expected to be of type \c CV_8U. Layout can be either
    2D or 3D. The returned value is a pointer to the beginning of a cell in one of the
    matrices, therefore the selected matrix must be guaranteed to exist for as long as
    the pointer is in use. Which cell of what matrix is selected depends on the interface's
    implementation.
    */
    virtual uint8_t *bitstring(const clarus::List<cv::Mat> &inputs) = 0;

    /**
    \brief Returns the distance between currently selected bit strings in the input and memory.

    In principle, for a control object <c>c</c>, this method should be equivalent to
    <c>Bitstring::distance(c.bistring(input), c.bitstring(inputs), Bitstring::bytes(input))</c>,
    but of course implementations are free to do as they please.
    */
    virtual uint32_t distance(const cv::Mat &input, const clarus::List<cv::Mat> &inputs) = 0;

    /**
    \brief Returns the current index associated to the given dimension.

    vgram::Layer objects expect control objects to have three dimensions, corresponding
    to bit string row, column and training case positions respectively.
    */
    virtual int index(int i) = 0;

    /**
    \brief Returns whether the control looped.

    This method signals to vgram::Layer objects that the control object has finished
    issuing all its index sets. Implementations are expected at this point to have already
    updated their internal states to enable a new run over the index set (see the documentation
    for #next()), therefore after a call that returns \c true, subsequent calls should return
    \c false until the control object once again goes through all possible index sets.
    */
    virtual bool more() = 0;

    /**
    \brief Moves the control to the next index set.

    The exact effect of this method will depend on the implementation, but the general
    idea is that it would update the object's internal state towards the next "step" in
    a progression of index sets. When called after the last set has been reached, it
    should set its internal state so that the next call to #more() will return \c false,
    and subsequent calls to #index(int) and #next() will run over the index set again
    from the beginning.
    */
    virtual void next() = 0;

    /**
    \brief Resizes the given dimension to the range <c>[v0, vn)</c>.

    If the corresponding current index falls outside the new range, the implementation
    should also update it to comply to the new range.
    */
    virtual void resize(int i, int v0, int vn) = 0;
};

#endif
