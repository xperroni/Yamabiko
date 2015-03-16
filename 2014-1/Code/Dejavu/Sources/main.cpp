
//#include "dejavu.hpp"

//#include "navigator.hpp"

/*
#include "scene_split_merge.hpp"

void test_scene_split_merge() {
    cv::Mat bgr = images::load("/home/helio/Projects/Data/Landmarks/stop-0-0-6-still.png");

    Scene scene(bgr);
    for (ListIteratorConst<cv::Rect> i(scene.segments); i.more();) {
        const cv::Rect &rect = i.next();
        cv::rectangle(bgr, rect, cv::Scalar(0, 0, 255));
    }

    viewer::show("image", bgr);
    cv::waitKey();
}
*/

#include "methods.hpp"

#include <boost/cstdint.hpp>

#include <limits>

double distance(uint32_t a, uint32_t b) {
    return __builtin_popcount(a ^ b);
}

uint32_t gray_code(int32_t value) {
    uint32_t a = ((long) value) - ((long) std::numeric_limits<int32_t>::min());
    return (a >> 1) ^ a;
}

void test_mc_code() {
    const double n = 600;
    const double a = std::numeric_limits<int>::min();
    const double z = std::numeric_limits<int>::max();
    const double t = (z - a) / (n - 1);

    cv::Mat d_g(n, n, CV_64F);
    //cv::Mat d_m(n, n, CV_64F);
    cv::Mat d_n(n, n, CV_64F);
    for (double i = 0; i < n; i++) {
        for (double j = 0; j < n; j++) {
            d_g.at<double>(i, j) = distance(gray_code(t * i + a), gray_code(t * j + a));
            //d_m.at<double>(i, j) = distance(dejavu::mc_code(t * i + a), dejavu::mc_code(t * j + a));
            d_n.at<double>(i, j) = 255 - t * (std::max(i, j) - std::min(i, j));
        }
    }

    viewer::show("Distances (N)", colors::discrete(d_n));
    viewer::show("Distances (Gray)", colors::discrete(d_g));
    //viewer::show("Distances (MC)", colors::discrete(d_m));
    cv::waitKey();

    images::save(colors::discrete(d_n), "/home/helio/Pictures/n.png");
    images::save(colors::discrete(d_g), "/home/helio/Pictures/g.png");
}

int demo_grayscale(int argc, char *argv[]) {
    std::string tag = "shift-50";
    int w = 25;

    std::string yaw = "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-";
    std::string shift = "/home/helio/Roboken/Data/Straight/2014-06-17-shift-";
/*
    plot_grayscale(yaw + "01-00", tag, ".png", w);
    plot_grayscale(shift + "01--10", tag, ".png", w);
    plot_grayscale(shift + "01-10", tag, ".png", w);
    plot_grayscale(shift + "01--25", tag, ".png", w);
    plot_grayscale(shift + "01-25", tag, ".png", w);
    plot_grayscale(shift + "01--50", tag, ".png", w);
    plot_grayscale(shift + "01-50", tag, ".png", w);

    plot_grayscale(yaw + "02-00", tag, ".png", w);
    plot_grayscale(yaw + "02-02", tag, ".png", w);
    plot_grayscale(yaw + "02-05", tag, ".png", w);
    plot_grayscale(shift + "02--25", tag, ".png", w);
    plot_grayscale(shift + "02-25", tag, ".png", w);
    plot_grayscale(shift + "02--50", tag, ".png", w);
    plot_grayscale(shift + "02-50", tag, ".png", w);

    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-01-0", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-01-2", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-01--2", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-01-5", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-01--5", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-01-10", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-01--10", tag, ".png", w);

    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-02-0", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-02-2", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-02--2", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-02-5", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-02--5", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-02-10", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-05-29-02--10", tag, ".png", w);

    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-04-23-01", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-04-23-05", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-04-23-07", tag, ".png", w);

    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-yaw-05-00", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-yaw-06-00", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-01-00", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-02-00", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-03-00", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-04-00", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-05-00", tag, ".png", w);
    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-06-00", tag, ".png", w);
*/

    plot_grayscale("/home/helio/Roboken/Data/Straight/2014-07-17-yaw-01-02", "shift-50", ".png", w);

    return 0;
}

int review_runs(int argc, char *argv[]) {
    std::string width = "shift-50";
    int size = 25;

    std::string shift = "/home/helio/Roboken/Data/Straight/2014-06-17-shift-";
    std::string yaw = "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-";
/*
    std::string shift2 = "/home/helio/Roboken/Data/Straight/2014-06-18-shift-";
    std::string yaw2 = "/home/helio/Roboken/Data/Straight/2014-06-18-yaw-";
    plot_eye_buffer(yaw2 + "01-00", width, ".png", size);
    plot_eye_buffer(shift2 + "01-100", width, ".png", size);
    plot_eye_buffer(shift2 + "01-200", width, ".png", size);
*/
/*
    plot_eye_buffer(yaw + "01-00", width, ".png", size);
    plot_eye_buffer(shift + "01--10", width, ".png", size);
    plot_eye_buffer(shift + "01-10", width, ".png", size);
    plot_eye_buffer(shift + "01--25", width, ".png", size);
    plot_eye_buffer(shift + "01-25", width, ".png", size);
    plot_eye_buffer(shift + "01--50", width, ".png", size);
    plot_eye_buffer(shift + "01-50", width, ".png", size);

    plot_eye_buffer(yaw + "02-00", width, ".png", size);
    plot_eye_buffer(yaw + "02-02", width, ".png", size);
    plot_eye_buffer(yaw + "02-05", width, ".png", size);
    plot_eye_buffer(shift + "02--25", width, ".png", size);
    plot_eye_buffer(shift + "02-25", width, ".png", size);
    plot_eye_buffer(shift + "02--50", width, ".png", size);
    plot_eye_buffer(shift + "02-50", width, ".png", size);

    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-01-0", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-01-2", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-01--2", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-01-5", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-01--5", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-01-10", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-01--10", width, ".png", size);
*/
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-02-0", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-02-2", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-02--2", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-02-5", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-02--5", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-02-10", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-05-29-02--10", width, ".png", size);
/*
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-04-23-01", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-04-23-05", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-04-23-07", width, ".png", size);

    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-yaw-05-00", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-yaw-06-00", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-01-00", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-02-00", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-03-00", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-04-00", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-05-00", width, ".png", size);
    plot_eye_buffer("/home/helio/Roboken/Data/Straight/2014-07-08-retrace-06-00", width, ".png", size);
*/
    return 0;
}

#include "drift_search.hpp"
using clarus::List;
using clarus::Point;
using dejavu::DriftSearch;

#include <iostream>
#include <fstream>

void compare_runs(
    const std::string &tag, double u,
    const std::string &folder1,
    const std::string &folder2
) {
    std::cout << folder2 << "/" << tag << std::endl;

    DriftSearch searcher(25, u, folder1, folder2);

    std::string path = folder2 + "/" + tag + ".txt";
    std::ofstream out(path.c_str());
    while (searcher.more()) {
        cv::Mat drifts = searcher.search(30);
        if (drifts.rows == 0) {
            continue;
        }

        std::cout << drifts << std::endl;
        out << drifts << std::endl;
    }
}

int compare_runs(int argc, char *argv[]) {
    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-02-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-02-50"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-02-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-02--50"
    );

/*
    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-01-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-01-10"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-01-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-01--10"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-01-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-01-25"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-01-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-01--25"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-01-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-01-50"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-06-17-yaw-01-00",
        "/home/helio/Roboken/Data/Straight/2014-06-17-shift-01--50"
    );
*/
/*
    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-2"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-01--2"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-5"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-01--5"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-10"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-01-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-01--10"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-2"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-5"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-10"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-02--2"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-02--5"
    );

    compare_runs(
        "template-125", 1.25,
        "/home/helio/Roboken/Data/Straight/2014-05-29-02-0",
        "/home/helio/Roboken/Data/Straight/2014-05-29-02--10"
    );
*/
    return 0;
}

#include <cight/drift_estimator.hpp>
using cight::Estimator;

#include <cight/drift_reduce.hpp>
using cight::reduce_slip;

#include <cight/replay_stream.hpp>
using cight::ReplayStream;

int demo_estimator(int argc, char *argv[]) {
    ReplayStream original("/home/helio/Roboken/Data/Straight/2014-05-29-01-0");
    Estimator estimator(50, 5, 25, 25, original);

    ReplayStream retrace("/home/helio/Roboken/Data/Straight/2014-05-29-01-10");
    reduce_slip reduce;
    while (retrace.active()) {
        cv::Mat responses = estimator(retrace);
        std::cout << responses << std::endl;
        //int index = reduce(estimator(retrace));
        //std::cout << index << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    return demo_grayscale(argc, argv);
}
