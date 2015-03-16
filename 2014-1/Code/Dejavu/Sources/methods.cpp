
#include "methods.hpp"
using clarus::List;
using clarus::ListIteratorConst;

#include "contours.hpp"

#include <cight/image_stream.hpp>
using cight::ImageStream;

#include <cight/transforms.hpp>

#include <cight/working_memory.hpp>
using cight::Memory;
using cight::average_edges;

#include <clarus/model/point.hpp>
using clarus::Point;
using clarus::Point2D;

#include <clarus/vision/depths.hpp>
#include <clarus/vision/sparse.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;
using std::cout;
using std::endl;

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <stdexcept>

Point sparsity_extremes(const cv::Mat &drift) {
    cv::Mat data;
    if (drift.channels() == 1) {
        data = drift;
    }
    else {
        int rows = drift.rows;
        data = drift.reshape(1, 3);
        cv::reduce(data, data, 0, CV_REDUCE_SUM);
        data = data.reshape(1, rows);
    }

    cv::Mat counts;
    cv::reduce(data, counts, 0, CV_REDUCE_SUM);

    cv::Point minPos, maxPos;
    cv::minMaxLoc(counts, NULL, NULL, &minPos, &maxPos);
    return Point2D(minPos.x, maxPos.x);
}

static cv::Scalar WHITE = cv::Scalar::all(255);

cv::Mat filters(const cv::Mat &data) {
    static cv::Mat element5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    static cv::Mat element10 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));

    cv::Mat entropy = (cv::abs((data - 0.5) * 2.0) <= 0.1);
    cv::dilate(entropy, entropy, element5);
    cv::erode(entropy, entropy, element10);

    // Calculates contour vectors for the boundaries extracted above.
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(
        entropy, contours, hierarchy,
        CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE
    );

    int n = contours.size();

    cv::Mat mask(data.size(), CV_8U, cv::Scalar::all(0));
    for (int i = 0; i < n; i++) {
        std::vector<cv::Point> &contour = contours[i];
        cv::Point2f center;
        float r = 0.0;
        cv::minEnclosingCircle(contour, center, r);
        if (r < 50 || r > 100) {
            continue;
        }

        cv::circle(mask, center, r, WHITE, -1);
    }

    return mask;
/*
    std::vector<std::vector<cv::Point> > hulls(n);
    for (int i = 0; i < n; i++) {
        cv::convexHull(cv::Mat(contours[i]), hulls[i], false);
    }

    cv::Mat mask(data.size(), CV_8U, cv::Scalar::all(0));
    for (int i = 0; i < n; i++) {
        // Draws the selected contour.
        cv::drawContours(
            mask, hulls, i, WHITE,
            CV_FILLED, 8, hierarchy, INT_MAX
        );
    }
*/
}

cv::Mat entropy_count(const cv::Mat &inputs) {
    return (cv::abs((inputs - 0.5) * 2.0) <= 0.1);
}

cv::Mat hebbian(const cv::Mat &inputs, double a) {
    static cv::Mat element5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    static cv::Mat element10 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));

    cv::Mat entropy = entropy_count(inputs);
    Contours contours(entropy, a);
    List<cv::Vec4i> &hierarchy = contours.hierarchy;
    List<int> &selected = contours.selected;

    cv::Mat mask(inputs.size(), CV_8U, cv::Scalar::all(0));
    for (ListIteratorConst<int> i(selected); i.more();) {
        cv::drawContours(mask, *contours, i.next(), WHITE, CV_FILLED, 8, *hierarchy, INT_MAX);
    }

    cv::Mat outputs;
    entropy.copyTo(outputs, mask);
    return outputs;
}

cv::Mat interest_mask(const cv::Mat &inputs, double a) {
    cv::Mat entropy = entropy_count(inputs);
    Contours contours(entropy, a);
    List<cv::Vec4i> &hierarchy = contours.hierarchy;
    List<int> &selected = contours.selected;

    cv::Mat mask(inputs.size(), CV_8U, cv::Scalar::all(0));
    for (ListIteratorConst<int> i(selected); i.more();) {
        cv::drawContours(mask, *contours, i.next(), WHITE, CV_FILLED, 8, *hierarchy, INT_MAX);
    }

    return mask;
}

cv::Mat interest_rects(const cv::Mat &inputs, double a) {
    static cv::Mat element5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    static cv::Mat element10 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));

    cv::Mat entropy = entropy_count(inputs);
    Contours contours(entropy, a);
    List<int> &selected = contours.selected;

    cv::Mat mask(inputs.size(), CV_8U, cv::Scalar::all(0));
    for (ListIteratorConst<int> i(selected); i.more();) {
        const Contour &contour = contours[i.next()];
        cv::Rect rect = cv::boundingRect(contour);
        cv::rectangle(mask, rect, WHITE, CV_FILLED);
    }

    return mask;
}

cv::Mat interest_circles(const cv::Mat &inputs, double s) {
    static cv::Mat element5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    static cv::Mat element10 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));

    cv::Mat entropy = entropy_count(inputs);
    Contours contours(entropy, 0.0);
    List<int> &selected = contours.selected;

    cv::Mat mask(inputs.size(), CV_8U, cv::Scalar::all(0));
    for (ListIteratorConst<int> i(selected); i.more();) {
        const Contour &contour = contours[i.next()];
        cv::Point2f center;
        float r = 0.0;
        cv::minEnclosingCircle(contour, center, r);
        if (r < s) {
            continue;
        }

        r = std::min(r, (float) (s * 2.0));
        cv::circle(mask, center, r, WHITE, -1);
    }

    return mask;
}

cv::Mat interest_fields(const cv::Mat &inputs, double a, float r) {
    cv::Mat points = hebbian(inputs, a);
    int rows = points.rows;
    int cols = points.cols;

    cv::Mat mask(rows, cols, CV_8U, cv::Scalar::all(0));

    uchar *buffer = points.ptr<uchar>();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++, buffer++) {
            if (*buffer > 0) {
                cv::Point2f center(j, i);
                cv::circle(mask, center, r, WHITE, -1);
            }
        }
    }

    return mask;
}

#include <ctime>

void plot_grayscale(
    const std::string &folder,
    const std::string &run,
    const std::string &ext,
    int size
) {
    viewer::window gui(folder);

    ImageStream images(folder);
    Memory eye(size);
    for (int i = 1; i < size && images.more(); ++i) {
        cv::Mat gray = colors::grayscale(cight::upper_half(images()));
        eye.record(gray);
    }

    fs::ofstream diff(folder + "/diff-" + run + ".txt");

    while (images.more()) {
        long t = images.current();
        cv::Mat gray = colors::grayscale(cight::upper_half(images()));
        eye.record(gray);

        cv::Mat drift = change_average(eye);

        //cv::Mat drift_bgr = depths::bgr(-drift);
        gui(drift, 600, 100, true);
        images::save(drift, folder + "/drift-" + types::to_string(t) + ".png", true);

        cv::Mat diffs = cight::column_histogram(drift, 50);
        cout << "(" << t << ", " << diffs << ")" << endl;
        diff << "(" << t << ", " << diffs << ")" << endl;
    }
}

void plot_eye_buffer(
    const std::string &folder,
    const std::string &run,
    const std::string &ext,
    int size
) {
    viewer::window gui(folder);

    ImageStream images(folder);
    Memory eye(size);
    for (int i = 1; i < size && images.more(); ++i) {
        cv::Mat bgr = cight::upper_half(images());
        eye.record(bgr);
    }

    fs::ofstream diff(folder + "/diff-" + run + ".txt");
    while (images.more()) {
        long t = images.current();
        cv::Mat bgr = cight::upper_half(images());
        eye.record(bgr);

        cv::Mat drift = average_edges(eye);
        cv::Mat drift_bgr = depths::bgr(-drift);
        gui(drift_bgr, 600, 100, true);
        images::save(drift_bgr, folder + "/drift-" + types::to_string(t) + ".png");

        cv::Mat diffs = cight::column_histogram(drift, 50);
        cout << "(" << t << ", " << diffs << ")" << endl;
        diff << "(" << t << ", " << diffs << ")" << endl;
    }
}

void eye_buffer_show(const std::string &folder, const std::string &ext) {
    std::map<long, std::string> files;
    for(fs::directory_iterator i(folder), n; i != n; ++i) {
        const fs::path &file = *i;
        if (file.extension() == ext) {
            std::string name = file.stem().native();
            long id = types::from_string<long>(name.substr(name.find_first_of('-') + 1));
            files[id] = file.native();
        }
    }

    for (std::map<long, std::string>::const_iterator i = files.begin(), n = files.end(); i != n; ++i) {
        cv::Mat bgr = filter::tantriggs(images::load(i->second));
        viewer::show("Tan-Triggs", filter::tantriggs(bgr));
        //cv::waitKey();
    }
}
