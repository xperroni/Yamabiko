#ifndef DEJAVU_DRIFT_SEARCH_HPP
#define DEJAVU_DRIFT_SEARCH_HPP

#include <cight/image_stream.hpp>
#include <cight/working_memory.hpp>

#include <clarus/core/list.hpp>
#include <clarus/model/point.hpp>

#include <string>

namespace dejavu {
    class DriftSearch;
}

class dejavu::DriftSearch {
    cight::ImageStream reference;

    cight::ImageStream retracing;

    cight::Memory eye;

    double u;

public:
    /*
    Creates a new drift searcher across the given buffers and gap window.
    */
    DriftSearch(size_t w, double u, const std::string &reference, const std::string &retracing);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~DriftSearch();

    cv::Mat search(float s);

    bool more() const;
};

#endif // DEJAVU_DRIFT_SEARCH_HPP

