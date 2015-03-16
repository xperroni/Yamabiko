#include <clarus/n2ram/node.hpp>
using n2ram::ByteString;
using n2ram::Node;

#include <clarus/core/list.hpp>
using clarus::List;

#define NDEBUG
#include <cassert>

Node::Node():
    slice(0)
{
    data.value = NULL;
}

Node::Node(const ByteString &bytes, int value):
    slice(bytes)
{
    data.value = new int(value);
}

inline void node_split(
    Node &node,
    const ByteString &bytes, size_t i, uint8_t byte_i,
    int value, size_t level, size_t n
) {
    Node::Children *children = NULL;
    if (node.slice.size() + level >= n) {
        assert(node.slice.size() + level == n);

        children = new Node::Children();
        Node &child = (*children)[byte_i];
        child.slice = ByteString(node.slice, i + 1, true);
        assert(child.slice.size() + level + i + 1 == n);
        child.data.value = node.data.value;
        node.data.children = children;
        node.slice.resize(i);
    }
    else {
        children = node.data.children;
    }

    int j = i + level;
    uint8_t byte_j = bytes[j];
    Node &child = (*children)[byte_j];
    child.slice = ByteString(bytes, j + 1, false);
    assert(child.slice.size() + j + 1 == n);
    child.data.value = new int(value);
}

static void record_step(Node &node, const ByteString &bytes, int value, size_t level, size_t n) {
    size_t j = level;
    ByteString &slice = node.slice;
    for (size_t i = 0, m = slice.size(); i < m; i++, j++) {
        uint8_t b = slice[i];
        if (bytes[j] != b) {
            node_split(node, bytes, i, b, value, level, n);
            return;
        }
    }

    // Leaf nodes have their values set at creation time, and we don't want to change
    // these, so if this is a leaf node, terminate without changing the tree.
    if (j >= n) {
        return;
    }

    Node::Children &children = *(node.data.children);
    uint8_t byte = bytes[j];
    if (children.count(byte) > 0) {
        record_step(children[byte], bytes, value, j + 1, n);
    }
    else {
        Node &child = children[byte];
        child.slice = ByteString(bytes, j + 1, false);
        assert(child.slice.size() + j + 1 == n);
        child.data.value = new int(value);
    }
}

void Node::record(const ByteString &bytes, int value) {
    record_step(*this, bytes, value, 0, bytes.size());
}

static int retrieve_step(
    Node &node, const ByteString &bytes,
    size_t level, size_t n,
    int &cost, int &least
) {
    cost += node.slice.distance(bytes, level);
    if (cost >= least) {
        // This result will be ignored.
        return 0;
    }

    level += node.slice.size();
    if (level >= n) {
        least = cost;
        return *node.data.value;
    }

    uint8_t byte = bytes[level];
    Node::Children &children = *(node.data.children);
    int cost_best = least;
    int result = 0;

    // Gives priority to exact matches, if they exist.
    if (children.count(byte) > 0) {
        Node &child1 = children[byte];
        int cost1 = cost;
        int result1 = retrieve_step(child1, bytes, level + 1, n, cost1, least);
        if (cost1 < cost_best) {
            cost_best = cost1;
            result = result1;
        }
    }

    for (Node::Children::iterator i = children.begin(), z = children.end(); i != z; ++i) {
        uint8_t byte_i = i->first;
        if (byte_i == byte) {
            // Avoid going down an exact match twice.
            continue;
        }

        Node &child_i = i->second;
        int cost_i = cost + __builtin_popcount(byte_i ^ byte);
        int result_i = retrieve_step(child_i, bytes, level + 1, n, cost_i, least);
        if (cost_i < cost_best) {
            cost_best = cost_i;
            result = result_i;
        }
    }

    cost = cost_best;
    return result;
}

int Node::retrieve(const ByteString &bytes) {
    int cost = 0;
    int least = INT_MAX;
    return retrieve_step(*this, bytes, 0, bytes.size(), cost, least);
}
