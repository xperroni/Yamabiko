#include <clarus/vgram/network.hpp>
using clarus::List;
using vgram::Network;

Network::Network() {
    // Nothing to do.
}

Network::~Network() {
    // Nothing to do.
}

void Network::record(const cv::Mat &input, const cv::Mat &output) {
    layers(0).record(input, output);
}

List<cv::Mat> Network::retrieve(const cv::Mat &input) {
    return layers(0).retrieve(input);
}
