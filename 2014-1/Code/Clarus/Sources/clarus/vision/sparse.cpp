#include <clarus/vision/sparse.hpp>
using clarus::List;
using clarus::ListIteratorConst;
using clarus::Point;
using clarus::Point3D;

static cv::Scalar ONE = cv::Scalar::all(1);

static cv::Scalar ZERO = cv::Scalar::all(0);

inline void square(cv::Mat &data, int i, int j, int spread, const cv::Scalar &all) {
    int x = std::max(0, j - spread);
    int y = std::max(0, i - spread);

    int l = spread * 2;
    int w = std::min(l, data.cols - x);
    int h = std::min(l, data.rows - y);

    cv::Rect region(x, y, w, h);

    cv::rectangle(data, region, all, CV_FILLED);
}

void sparse::mask(cv::Mat &mask, const Point &point, int spread) {
    int i = point[0];
    int j = point[1];
    square(mask, i, j, spread, ONE);
}

cv::Mat sparse::mask(const cv::Size &size, const Point &point, int spread) {
    cv::Mat mask(size, CV_8U, ZERO);
    sparse::mask(mask, point, spread);
    return mask;
}

cv::Mat sparse::mask(const cv::Size &size, const List<Point> &points, int spread) {
    cv::Mat mask(size, CV_8U, ZERO);
    for (ListIteratorConst<Point> k(points); k.more();) {
        sparse::mask(mask, k.next(), spread);
    }

    return mask;
}

static bool pixel_order(const Point &a, const Point &b) {
    return a[2] > b[2];
}

List<Point> sparse::search(const cv::Mat &data, int spread, int upto) {
    List<Point> pixels;
    int rows = data.rows;
    int cols = data.cols;
    for (int i = 0; i < rows; i++) {
        const double *row = data.ptr<double>(i);
        for (int j = 0; j < cols; j++, row++) {
            pixels.append(Point3D(i, j, *row));
        }
    }

    clarus::sort(pixels, pixel_order);

    int z = 0;
    int d = spread * 2;
    List<Point> maxima;
    cv::Mat active(data.size(), CV_8U, cv::Scalar::all(1));
    for (ListIteratorConst<Point> k(pixels); k.more() && z < upto;) {
        const Point &point = k.next();
        int i = point[0];
        int j = point[1];
        if (active.at<uint8_t>(i, j) == 1) {
            square(active, i, j, d, ZERO);
            maxima.append(point);
            z++;
        }
    }

    return maxima;
}
