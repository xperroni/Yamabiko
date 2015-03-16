#ifndef DEJAVU_DRIFT_ESTIMATOR_H
#define DEJAVU_DRIFT_ESTIMATOR_H

#include <cight/sensor_stream.hpp>
#include <cight/working_memory.hpp>

#include <clarus/core/list.hpp>

#include <string>

namespace cight {
    class Estimator;
}

class cight::Estimator {
    clarus::List<cv::Mat> changes;

    cv::Mat odometry;

    Memory retrace;

    int bins;

    int window;

    int index;

public:
    /*
    Creates a new drift estimator, using the giving folder for storage.
    */
    Estimator(int bins, int window, size_t range_t, size_t range_r, SensorStream &sensors);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Estimator();

    /*
    Calculates a vector of drift estimate likelihoods.
    */
    cv::Mat operator () (SensorStream &sensors);

    /*
    Erases the estimator's memory, forcing it to rebuild its image base on the next
    call.
    */
    void reset();

    /*
    Returns whether the current estimated position is the last known one.
    */
    bool arrived();
};

#endif
