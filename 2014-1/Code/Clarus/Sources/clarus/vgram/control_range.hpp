#ifndef CLARUS_VGRAM_CONTROL_RANGE_HPP
#define CLARUS_VGRAM_CONTROL_RANGE_HPP

#include <clarus/vgram/control.hpp>

#include <boost/smart_ptr.hpp>

namespace vgram {
    struct ControlRange;
}

/**
Control object for restricting queries within a range of memories in a VG-RAM layer.


*/
struct vgram::ControlRange: public Control {
    /** \brief Smart (reference-counted) pointer type. */
    typedef boost::shared_ptr<ControlRange> P;

    /** \brief Array of current index positions. */
    int indices[3];

    /** \brief Array of index start values. */
    int start[3];

    /** \brief Array of index limit values (one plus the maximum value). */
    int stop[3];

    /** \brief Flag indicating whether a full range loop has been performed. */
    bool looped;

    /**
    \brief Default constructor.
    */
    ControlRange();

    /**
    \brief Creates a new control object with given ranges.

    \param i0 Initial value for the row range.

    \param in One past the final value for the row range.

    \param j0 Initial value for the column range.

    \param jn One past the final value for the column range.

    \param z0 Initial value for the training case range.

    \param zn One past the final value for the training case range.
    */
    ControlRange(int i0, int in, int j0, int jn, int z0, int zn);

    /**
    \brief Creates a new control with ranges from 0 to the given 3D matrix size dimensions.

    This constructor is equivalent to <c>ControlRange(0, size[0], 0, size[1], 0, size[2])<\c>.
    */
    ControlRange(const cv::Mat::MSize &size);

    /**
    \brief Virtual destructor.

    Enforces polymorphism. Do not remove.
    */
    virtual ~ControlRange();

    /**
    \brief Returns the bit string at <c>input[index(0), index(1)]</c>.

    \see vgram::Control::bitstring(const cv::Mat&)
    */
    virtual uint8_t *bitstring(const cv::Mat &input);

    /**
    \brief Returns the bit string at <c>inputs[index(2)][index(0), index(1)]</c>.

    \see vgram::Control::bitstring(const clarus::List<cv::Mat>&)
    */
    virtual uint8_t *bitstring(const clarus::List<cv::Mat> &inputs);

    /**
    \brief Computes the distances between currently selected bit strings in the given
    query input and layer memory.

    For a vgram::ControlRange object \c c, this method is equivalent to
    <c>Bitstring::distance(c.bistring(input), c.bitstring(inputs), Bitstring::bytes(input))</c>.

    \see vgram::Control::distance(const cv::Mat&, const clarus::List<cv::Mat>&)
    */
    virtual uint32_t distance(const cv::Mat &input, const clarus::List<cv::Mat> &inputs);

    /**
    \brief Returns the current index associated to the given dimension.

    For a vgram::ControlRange object \c c, this method is equivalent to <c>c.indices[i]<\c>.

    \see vgram::Control::index(int)
    */
    virtual int index(int i);

    /**
    \brief Returns whether the control looped.

    This method returns the value of the #looped flag, which is set after all valid index
    triples in the current range have been visited (see #next() for more details). If
    #looped is true, then this method resets it (i.e. makes <c>looped = false</c>) and
    returns \c true. Otherwise it returns \c false.

    \see vgram::Control::more()
    */
    virtual bool more();

    /**
    \brief Moves the control to the next index set.

    Every time this method is called indices are updated in this manner:

    <ol>
        <li>Increment the column index (<c>indices[1]</c>);</li>
        <li>
            If <c>indices[1] \< stop[1]</c> return, otherwise set
            <c>indices[1] = start[1]</c> and go to the next step;
        </li>
        <li>Increment the row index (<c>indices[0]</c>);</li>
        <li>
            If <c>indices[0] \< stop[0]</c> return, otherwise set
            <c>indices[0] = start[0]</c> and go to the next step;
        </li>
        <li>Increment the training case index (<c>indices[2]</c>);</li>
        <li>
            If <c>indices[2] \< stop[2]</c> return, otherwise set
            <c>indices[2] = start[2]</c> and <c>looped = true</c>, then return.
        </li>
    </ol>

    \see vgram::Control::next()
    */
    virtual void next();

    /**
    \brief Resizes the given dimension to the range <c>[v0, vn)</c>.

    If <c>indices[i] \< v0</code> then also make <c>indices[i] = v0</code>.

    Likewise, if <c>indices[i] >= vn</code> then also make <c>indices[i] = vn - 1</code>.

    \see vgram::Control::resize(int, int, int)
    */
    virtual void resize(int i, int v0, int vn);
};

#endif
