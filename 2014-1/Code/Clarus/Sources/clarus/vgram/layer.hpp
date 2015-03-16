#ifndef CLARUS_VGRAM_LAYER_HPP
#define CLARUS_VGRAM_LAYER_HPP

#include <clarus/core/list.hpp>
#include <clarus/vgram/control.hpp>

#include <boost/smart_ptr.hpp>

#include <opencv2/opencv.hpp>

namespace vgram {
    struct Layer;
}

/**
A single layer in a VG-RAM network. It represents a rectangular matrix of VG-RAM
neurons. Training and querying operations access all neurons in the layer at once.
This design provides the best balance between performance and flexibility, and is
convenient to most known applications of VG-RAM networks.

The basic VG-RAM design is seldom sufficient for most real-world applications; it
is common for applications to add their own customizations to the standard method.
For this reason this class was designed with extensibility in mind: all its members
are public, and all methods are virtual.

In the current implementation, layers expect inputs to always be 3D matrices of
type \c CV_8U and dimensions compatible to the layer itself (i.e. same height, width
and depth as the layer), and outputs to always be 2D matrices of type \c CV_8U and
compatible width and height. However no checks are performed to ensure this is ever
the case. This is by design, as purpose-written subclasses may loosen these restrictions.
*/
struct vgram::Layer {
    /** \brief Reference-counted pointer class. */
    typedef boost::shared_ptr<Layer> P;

    /**
    \brief Layer dimensions.

    The first and second dimensions correspond to the height and width of inputs and
    outputs. The third dimension is the length of input bit strings, in bytes.
    */
    const cv::Mat::MSize size;

    /**
    \brief Input bit strings stored in the layer.

    Each element in the list is a 3D matrix of the same dimensions as specified in
    vgram::Layer::size and type \c CV_8U.
    */
    clarus::List<cv::Mat> inputs;

    /**
    \brief Output values stored in the layer.

    Each element is a 2D matrix of same dimensions as the first two values of
    vgram::Layer::size. Currently only outputs of type \c CV_8U are supported.
    */
    clarus::List<cv::Mat> outputs;

    /**
    Creates a new layer of given width and height, storing bit strings of the given
    length (in bytes).
    */
    Layer(size_t width, size_t height, size_t bytes);

    /**
    \brief Virtual destructor.

    Enforces polymorphism. Do not remove.
    */
    virtual ~Layer();

    /**
    \brief Stores the given (input, output) pair to this layer.

    Both input and output matrices are expected to comply to layer dimension and type
    restrictions (see the class description for details), but no checks are performed
    to ensure this is ever the case. Non-compliant arguments will lead to undefined
    behavior in later retrieve operations, unless a subclass override has loosened
    these restrictions somehow.
    */
    virtual void record(const cv::Mat &input, const cv::Mat &output);

    /**
    \brief Query this layer for the given input, returning the associated values.

    The returned list contains three 3D matrices, containing the selected output, matching
    distance and index of the best matching bit string for each layer neuron. The output
    matrix is of type \c CV_8U, while the distance and index matrices are of type \c CV_32S.

    If <c>top > 1</c> then the three matrices contain as many depth levels, containing the
    corresponding results for as many best matches. Be aware that matches are not ordered
    in any meaningful way (e.g. matches at the same depth level do not necessarily have
    the same relative match quality).
    */
    virtual clarus::List<cv::Mat> retrieve(const cv::Mat &input, int top = 1);

    /**
    \brief Query this layer for the given input, returning the associated values. The search
    is restricted according to the given control object.

    The returned list contains three 3D matrices, containing the selected output, matching
    distance and index of the best matching bit string for each layer neuron. The output
    matrix is of type \c CV_8U, while the distance and index matrices are of type \c CV_32S.

    If <c>top > 1</c> then the three matrices contain as many depth levels, containing the
    corresponding results for as many best matches. Be aware that matches are not ordered
    in any meaningful way (e.g. matches at the same depth level do not necessarily have
    the same relative match quality).
    */
    virtual clarus::List<cv::Mat> retrieve(const cv::Mat &input, Control &control, int top = 1);

    /**
    Returns the total expected size of layer inputs, in bytes.
    */
    virtual uint32_t total() const;
};

#endif
