#ifndef CLARUS_VGRAM_LAYER_EXTENSIONS_HPP
#define CLARUS_VGRAM_LAYER_EXTENSIONS_HPP

#include <clarus/vgram/layer.hpp>

namespace vgram {
    /**
    \brief Compares the given input to each layer input record as a whole.

    Returns the index to the best match within the given range <c>[r0, rn)</c>.
    */
    int relate(const Layer &layer, const cv::Mat &input, int r0, int rn);

    /**
    \brief Compares the bit string at the given input position to the whole layer memory.

    Returns the coordinates of the best match.
    */
    cv::Point3i relate(const Layer &layer, const cv::Mat &input, const cv::Point2i &focus);

    /**
    \brief Compares the bit string at the given input position to the whole layer memory,
    subject to the given control.

    Returns the coordinates of the best match.
    */
    cv::Point3i relate(const Layer &layer, const cv::Mat &input, const cv::Point2i &focus, Control &control);

    /**
    \brief Compares the given input to each layer record as a whole.

    Returns a list of indices to the best n matches.
    */
    clarus::List<int> relate(const Layer &layer, const cv::Mat &input, int n);

    /**
    \brief Compares the given regions of the input to corresponding sections of each layer record.

    Returns a list of indices to the best matches.
    */
    clarus::List<int> relate(const Layer &layer, const cv::Mat &input, const clarus::List<cv::Rect> &rects);
}

#endif
