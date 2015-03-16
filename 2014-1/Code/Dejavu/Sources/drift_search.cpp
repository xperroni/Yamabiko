#include "drift_search.hpp"
using clarus::List;
using clarus::ListIteratorConst;
using clarus::Point;
using clarus::Point2D;
using clarus::Point3D;
using dejavu::DriftSearch;

#include "contours.hpp"

#include <clarus/io/viewer.hpp>
#include <clarus/vision/colors.hpp>
#include <clarus/vision/filters.hpp>
#include <clarus/vision/sparse.hpp>

static cv::Scalar BLACK = cv::Scalar::all(0);

static cv::Scalar WHITE = cv::Scalar::all(255);

DriftSearch::DriftSearch(
    size_t w, double _u,
    const std::string &_reference,
    const std::string &_retracing
):
    reference(_reference),
    retracing(_retracing),
    eye(w),
    u(_u)
{
    for (size_t i = 1; reference.more() && i < w; i++) {
        cv::Mat bgr = reference();
        eye.append(bgr);

        // Discard an image from the retracing run to keep it in sync with the reference
        retracing();
    }
}

DriftSearch::~DriftSearch() {
    // Nothing to do.
}

struct Target {
    cv::Point2f center;

    float r;

    Target(const Contour &contour, float s) {
        cv::minEnclosingCircle(contour, center, r);
        r = std::min(r, (float) (s * 2.0));
    }
};

static cv::Mat extract_neighborhood(const cv::Mat &image, const cv::Point2f &center, float r) {
    int length = ceil(r);
    int col_0 = std::max(center.x - length, 0.0f);
    int col_l = std::min(center.x + length, (float) image.cols) - col_0;
    int row_0 = std::max(center.y - length, 0.0f);
    int row_l = std::min(center.y + length, (float) image.rows) - row_0;

    cv::Rect roi(col_0, row_0, col_l, row_l);
    cv::Mat roid(image, roi);

    cv::Mat cropped;
    roid.copyTo(cropped);
    return cropped;
}

static cv::Mat extract_circle(const cv::Mat &image, const Target &target) {
    cv::Mat mask(image.size(), CV_8U, BLACK);
    cv::circle(mask, target.center, target.r, WHITE, -1);

    cv::Mat masked;
    image.copyTo(masked, mask);

    return extract_neighborhood(masked, target.center, target.r);
}

static cv::Mat extract_square(const cv::Mat &image, const Point &target, int r) {
    int l = r * 2;

    int x = std::max(0.0, target[1] - r);
    if (x + l > image.cols) {
        x = image.cols - l;
    }

    int y = std::max(0.0, target[0] - r);
    if (y + l > image.rows) {
        y = image.rows - l;
    }

    cv::Rect roi(x, y, l, l);

    cv::Mat square(image, roi);
    return square;
}

static cv::Mat templateSearch(const cv::Mat &image, const cv::Mat &templ, const int range) {
    static viewer::window gui("Templates");

    cv::Mat matches;
    cv::matchTemplate(image, templ, matches, CV_TM_CCORR_NORMED);
    gui(matches, -1, -1, true);

    double maxVal = 0.0;
    cv::Point maxLoc(0, 0);
    cv::minMaxLoc(matches, NULL, &maxVal, NULL, &maxLoc);
    int maxIndex = maxLoc.x + (range - image.cols + templ.cols) / 2;

    cv::Mat results(1, range, matches.type(), cv::Scalar::all(0));
    results.at<float>(0, maxIndex) = maxVal;
    return results;
}

static cv::Mat templateSearch(const cv::Mat &image, const cv::Mat &templ) {
    static viewer::window gui("Templates");

    cv::Mat matches;
    cv::matchTemplate(image, templ, matches, CV_TM_CCORR_NORMED);
    gui(matches, -1, -1, true);

    double maxVal = 0.0;
    cv::Point maxLoc(0, 0);
    cv::minMaxLoc(matches, NULL, &maxVal, NULL, &maxLoc);
    int maxIndex = maxLoc.x + templ.cols / 2;

    cv::Mat results(1, image.cols, CV_32F, cv::Scalar::all(0));
    results.at<float>(0, maxIndex) = maxVal;
    return results;
}

static cv::Mat templateSearch(const cv::Mat &templ, const cv::Mat &refer, const cv::Mat &close) {
    static viewer::window gui_refer("Reference Search");
    static viewer::window gui_drift("Drift Search");

    cv::Mat matches;
    cv::Point referLoc(0, 0);
    cv::matchTemplate(refer, templ, matches, CV_TM_CCORR_NORMED);
    cv::minMaxLoc(matches, NULL, NULL, NULL, &referLoc);
    gui_refer(matches, -1, -1, true);

    cv::matchTemplate(close, templ, matches, CV_TM_CCORR_NORMED);
    gui_drift(matches, -1, -1, true);

    int cols = close.cols;

    double maxVal = 0.0;
    cv::Point maxLoc(0, 0);
    cv::minMaxLoc(matches, NULL, &maxVal, NULL, &maxLoc);
    int maxIndex = std::min(std::max(0, maxLoc.x - referLoc.x + cols / 2), cols - 1);

    cv::Mat results(1, cols, CV_32F, cv::Scalar::all(0));
    results.at<float>(0, maxIndex) = maxVal;
    return results;
}

/*
static cv::Mat templateSearch(const cv::Mat &image, const cv::Mat &templ, const int range) {
    cv::Mat matches;
    cv::matchTemplate(image, templ, matches, CV_TM_CCORR_NORMED);

    cv::Mat totals;
    cv::reduce(matches, totals, 0, CV_REDUCE_SUM);

    cv::Mat results(1, range, matches.type(), cv::Scalar::all(0));
    int shift = (range - image.cols + templ.cols) / 2;
    cv::Rect roi(shift, 0, totals.cols, 1);
    cv::Mat roid(results, roi);
    totals.copyTo(roid);
    return results;

//    cv::Mat results(1, range, matches.type(), cv::Scalar::all(0));
//    int shift = (range - image.cols + templ.cols) / 2;
//
//    cv::Rect roi(shift, 0, totals.cols, 1);
//    cv::Mat roid(results, roi);
//    totals.copyTo(roid);
//    return results;
}
*/

static cv::Mat upper_half(const cv::Mat &inputs) {
    int rows = inputs.rows;
    int cols = inputs.cols;

    cv::Rect roi(0, 0, cols, rows / 2);
    cv::Mat half(inputs, roi);
    return half;
}

cv::Mat DriftSearch::search(float s) {
    static viewer::window gui_close("Neighborhood");
    static viewer::window gui_templ("Target");

    if (!more()) {
        return cv::Mat();
    }

    cv::Mat referd = reference();
    cv::Mat sought = retracing();

    eye.append(referd);

    cv::Mat e_referd = filter::sobel(colors::grayscale(referd));
    cv::Mat e_sought = filter::sobel(colors::grayscale(sought));

    cv::Mat upper = upper_half(average_edges(eye));
    //cv::Mat count = (cv::abs((average - 0.5) * 2.0) <= 0.1);
    List<Point> regions = sparse::search(upper, 16, 20);
    if (regions.size() == 0) {
        return cv::Mat();
    }

    cv::Mat drifts(1, 256, CV_32F, cv::Scalar::all(0));
    for (ListIteratorConst<Point> i(regions); i.more();) {
        const Point &target = i.next();

        cv::Mat close = extract_square(sought, target, 128);
        cv::Mat refer = extract_square(referd, target, 128);
        cv::Mat templ = extract_square(referd, target, 32);
        if (cv::norm(cv::sum(templ)) < 50000.0) {
            continue;
        }

        drifts += templateSearch(templ, refer, close);

        gui_close(close);
        gui_templ(templ);
        //cv::waitKey();

/*
        std::cout << "Neighborhood " << Point2D(close.cols, close.rows) << std::endl;
        std::cout << "Target " << Point2D(templ.cols, templ.rows) << std::endl;
        std::cout << drift << std::endl << std::endl;

        viewer::show("Neighborhood", close);
        viewer::show("Target", templ);
        cv::waitKey();
*/
    }

    return drifts;
}

bool DriftSearch::more() const {
    return (reference.more() && retracing.more());
}
