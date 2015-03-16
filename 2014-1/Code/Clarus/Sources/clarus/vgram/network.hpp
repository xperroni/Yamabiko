#ifndef CLARUS_VGRAM_NETWORK_HPP
#define CLARUS_VGRAM_NETWORK_HPP

#include <clarus/core/list.hpp>
#include <clarus/vgram/layer.hpp>

#include <opencv2/opencv.hpp>

namespace vgram {
    struct Network;
}

/**
Base class for a VG-RAM network. Maintains a list of layers and includes methods
for recording training cases to them and reading estimates back.

While the Network class is concrete and could be instantiated, it is not expected
to ever be used on its own, but rather serve as the parent of application-specific
subclasses. For this reason all its methods are virtual. In the same spirit, all
its members are public to simplify the implementation of variations to the basic
VG-RAM network model.
*/
struct vgram::Network {
    /**
    \brief List of layers included in this network.

    In order to enable subclasses of vgram::Layer to be added to the network, it is
    necessary to maintain a list of pointers, rather than store Layer objects directly.
    This however implies managing dynamically-allocated objects (in particular, deleting
    them when appropriate). It also makes the syntax more verbose than when objects are
    accessed through references.

    To address both concerns, class vgram::Network::L implements a list of shared pointers
    to vgram::Layer objects, adding a functional operator to access stored elements by
    reference (the indexing operator \c [] can still be used to access shared pointer
    entries).
    */
    struct L: public clarus::List<Layer::P> {
        /**
        \brief Returns a reference to the layer of given index.

        \throw std::out_of_range If the index is invalid.
        */
        Layer &operator () (size_t index) {
            Layer::P l = this->at(index);
            return *l;
        }

        /**
        \brief Adds a new vgram::Layer object to the list.

        Returns a reference to the added object.
        */
        Layer &append(Layer *layer) {
            clarus::List<Layer::P>::append(Layer::P(layer));
            return *layer;
        }
    };

    /** \brief List of VG-RAM layers that compose this network. */
    Network::L layers;

    /**
    \brief Creates a new network.

    The default constructor will create a network with no layers. Subclasses are encouraged
    to define constructors that initialize the network according to their own specifications.
    */
    Network();

    /**
    \brief Virtual destructor.

    Enforces polymorphism. Do not remove.
    */
    virtual ~Network();

    /**
    \brief Feed the given (input, output) pair to the network for learning.

    The default implementation just passes its arguments over to the first layer in the
    network. Notice that if the arguments do not comply with the layer's specifications
    (i.e. its width, height and bit string length) this will lead to undefined behavior
    when the layer's \c retrieve() method is called.

    Subclasses are encouraged to override this method with an implementation better
    suited to their own specifications.

    \param input Matrix of bit strings, i.e. a 3D matrix of type CV_8U. Dimensions are
                 presumed compatible with this network.

    \param output Matrix of output values. Currently only type CV_8U is supported.
    */
    virtual void record(const cv::Mat &input, const cv::Mat &output);

    /**
    \brief Queries the network with the given input.

    The default implementation just passes its arguments over to the first layer in the
    network. Notice that if input does not comply with the layer's specifications
    (i.e. its width, height and bit string length) this will lead to undefined behavior
    when the layer's \c retrieve() method is called.

    Subclasses are encouraged to override this method with an implementation better
    suited to their own specifications.

    \param input Matrix of bit strings, i.e. a 3D matrix of type \c CV_8U. Dimensions are
                 presumed compatible with this network.

    \return Matrix of output values. Currently only type \c CV_8U is supported.
    */
    virtual clarus::List<cv::Mat> retrieve(const cv::Mat &input);
};

#endif
