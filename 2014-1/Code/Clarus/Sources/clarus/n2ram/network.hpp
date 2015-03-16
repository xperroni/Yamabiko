#ifndef CLARUS_N2RAM_NETWORK_HPP
#define CLARUS_N2RAM_NETWORK_HPP

#include <clarus/n2ram/node.hpp>

#include <boost/shared_array.hpp>

#include <opencv2/opencv.hpp>

namespace n2ram {
    struct Network;
}

struct n2ram::Network {
/*
    typedef boost::shared_array<uint8_t> Path;

    List<Path> paths;

    List<uint8_t> values;
*/
    Node root;

    /*
    Creates a new network of given depth.
    */
    Network(size_t depth);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Network();

    virtual void record(const cv::Mat &inputs, const cv::Mat &outputs);

    virtual cv::Mat retrieve(const cv::Mat &inputs);
};

#endif
