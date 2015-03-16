#include "viewer.hpp"

#include <cight/image_stream.hpp>
using cight::ImageStream;

#include <clarus/io/viewer.hpp>
#include <clarus/vision/colors.hpp>
#include <clarus/vision/cvmat.hpp>
#include <clarus/vision/filters.hpp>
#include <clarus/vision/sparse.hpp>

#include <string>

void view_sobel(const std::string &folder) {
    viewer::window gui(folder);

    ImageStream images(folder);
    while (images.more()) {
        cv::Mat bgr = images();
        cv::Mat e = filter::sobel(colors::grayscale(bgr));
        cv::Mat mask = sparse::mask(e.size(), sparse::search(e, 32, 20), 32);

        cv::Mat out;
        e.copyTo(out, mask);

        gui(out, 600, 100, true);
        //cv::waitKey(300);
    }
}

int view_sobel(int argc, char *argv[]) {
    view_sobel("/home/helio/Roboken/Data/Straight/2014-05-29-01-0");
    return 0;
}
