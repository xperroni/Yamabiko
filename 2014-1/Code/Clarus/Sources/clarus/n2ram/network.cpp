#include <clarus/n2ram/network.hpp>
using n2ram::ByteString;
using n2ram::Network;

#include <cstring>

Network::Network(size_t depth):
    root(ByteString(depth), 0)
{
    // Nothing to do.
}

Network::~Network() {
    // Nothing to do.
}
/*
void Network::record(const cv::Mat &inputs, const cv::Mat &outputs) {
    int rows = inputs.size[0];
    int cols = inputs.size[1];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Bitstring bitstring(inputs, i, j);
            int value = outputs.at<uint8_t>(i, j);

            size_t n = bitstring.size / 8;
            Path path(new uint8_t[n]);
            memcpy(path.get(), bitstring.bits, n);

            paths.append(path);
            values.append(value);
        }
    }
}

cv::Mat Network::retrieve(const cv::Mat &inputs) {
    int rows = inputs.size[0];
    int cols = inputs.size[1];
    cv::Mat outputs(rows, cols, CV_32S);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Bitstring bitstring(inputs, i, j);
            outputs.at<uint8_t>(i, j) = root.retrieve(bitstring);
        }
    }

    return outputs;
}
*/
void Network::record(const cv::Mat &inputs, const cv::Mat &outputs) {
    int rows = inputs.size[0];
    int cols = inputs.size[1];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            ByteString bytes(inputs, i, j);
            int value = outputs.at<uint8_t>(i, j);
            root.record(bytes, value);
        }
    }
}

cv::Mat Network::retrieve(const cv::Mat &inputs) {
    int rows = inputs.size[0];
    int cols = inputs.size[1];
    cv::Mat outputs(rows, cols, CV_8U);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            ByteString bytes(inputs, i, j);
            outputs.at<uint8_t>(i, j) = root.retrieve(bytes);
        }
    }

    return outputs;
}
