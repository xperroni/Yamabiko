#include <clarus/core/list.hpp>

template<> List<cv::Mat> List<cv::Mat>::clone(bool deep) const {

    List cloned;
    if (deep) {
        Buffer &clones = cloned.buffer;
        for (typename Buffer::const_iterator i = buffer.begin(), n = buffer.end(); i != n; ++i) {
            clones.push_back(i->clone());
        }
    }
    else {
        cloned.buffer = buffer;
    }

    return cloned;
}
