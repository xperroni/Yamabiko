#include <cight/drift_estimator.hpp>
using cight::Estimator;

#include <cight/transforms.hpp>

#include <clarus/core/list.hpp>
using clarus::List;

#include <clarus/vision/colors.hpp>
#include <clarus/vision/fourier.hpp>

inline cv::Mat preprocess(const cv::Mat &image) {
    return colors::grayscale(cight::upper_half(image));
}

inline void fill_up(cight::Memory &memory, cight::SensorStream &sensors) {
    while (memory.idle() > 1 && sensors.active()) {
        cight::SensorStream::S s = sensors();
        memory.record(preprocess(s.image));
    }
}

Estimator::Estimator(int _bins, int _window, size_t range_t, size_t range_r, SensorStream &replay):
    retrace(range_r),
    bins(_bins),
    window(_window),
    index(0)
{
    Memory original(range_t);
    fill_up(original, replay);

    List<double> readings;
    while (replay.active()) {
        SensorStream::S reading = replay();
        original.record(preprocess(reading.image));
        changes.append(column_histogram(change_average(original), bins));
        readings.append(reading.x);
    }

    odometry = cv::Mat(1, readings.size(), CV_64F);
    for (int i = 0, n = readings.size(); i < n; i++) {
        odometry.at<double>(0, i) = readings[i];
    }
}

Estimator::~Estimator() {
    // Nothing to do.
}

cv::Mat Estimator::operator () (SensorStream &sensors) {
    fill_up(retrace, sensors);

    SensorStream::S reading = sensors();
    retrace.record(preprocess(reading.image));
    cv::Mat change = column_histogram(change_average(retrace), bins);

    cv::Point minLoc;
    cv::Mat diffs = cv::abs(odometry - reading.x);
    cv::minMaxLoc(diffs, NULL, NULL, &minLoc);
    index = std::max(index, minLoc.x);

    cv::Mat closest = changes[index];

    int n = closest.cols;
    cv::Mat responses(1, n * 2, CV_64F, cv::Scalar::all(0));
    for (int i = 0, m = 1 + n - window; i < m; i++) {
        cv::Mat b(closest, cv::Rect(i, 0, window, 1));
        cv::Mat c = fourier::correlate(change, b);
        cv::Mat r(responses, cv::Rect(n - i, 0, n, 1));
        r += c;
    }

    return responses;
}

void Estimator::reset() {
    retrace.clear();
}

bool Estimator::arrived() {
    return (index == odometry.cols - 1);
}
