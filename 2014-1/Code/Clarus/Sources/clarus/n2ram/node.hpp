#ifndef CLARUS_N2RAM_NODE_HPP
#define CLARUS_N2RAM_NODE_HPP

#include <clarus/n2ram/byte_string.hpp>

#include <boost/shared_ptr.hpp>

#include <map>

namespace n2ram {
    struct Node;
}

struct n2ram::Node {
    typedef std::map<uint8_t, Node> Children;

    ByteString slice;

    union {
        Children *children;
        int *value;
    } data;

    /*
    Default constructor.
    */
    Node();

    /*
    Creates a new node with given bit string and value.
    */
    Node(const ByteString &bytes, int value);

    void record(const ByteString &bytes, int value);

    int retrieve(const ByteString &bytes);
};

#endif
