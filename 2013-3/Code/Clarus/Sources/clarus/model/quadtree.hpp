#ifndef CLARUS_MODEL_QUADTREE_HPP
#define CLARUS_MODEL_QUADTREE_HPP

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <stdexcept>
#include <string>

template<class T> class Quadtree {
    struct node {
        typedef boost::shared_ptr<node> nodeP;

        typedef List<nodeP> Children;

        typedef std::pair<cv::Point, T> Value;

        typedef List<Value> Values;

        cv::Rect bounds;

        Children children;

        Values values;

        node(const cv::Rect &bounds);

        bool add(const cv::Point &p, const T& value);

        void query(const cv::Rect &range, Values &result) const;
    };

    node root;

public:
    typedef typename node::Values Values;

    Quadtree(const cv::Size &size);

    void add(int x, int y, const T &value);

    void add(const cv::Point &point, const T &value);

    Values query(const cv::Rect &range) const;

    const cv::Size &size() const;
};

template<class T> Quadtree<T>::node::node(const cv::Rect &_bounds):
    bounds(_bounds)
{
    // Nothing to do.
}

template<class T> bool Quadtree<T>::node::add(const cv::Point &point, const T &value) {
    if (!point.inside(bounds)) {
        return false;
    }

    if (values.size() < 4) {
        values.append(std::make_pair(point, value));
        return true;
    }

    if (children.size() == 0) {
        int xn = bounds.width;
        int yn = bounds.height;
        int xm = xn / 2;
        int ym = yn / 2;

        int x0 = bounds.x;
        int y0 = bounds.y;
        int xk = x0 + xm;
        int yk = y0 + ym;

        children.append().reset(new node(cv::Rect(x0, y0, xm, ym)));
        children.append().reset(new node(cv::Rect(xk, y0, xn, ym)));
        children.append().reset(new node(cv::Rect(x0, yk, xm, yn)));
        children.append().reset(new node(cv::Rect(xk, yk, xn, yn)));
    }

    for (ListIterator<node> i(children); i.more();) {
        nodeP &child = i.next();
        if (child->add(point, value)) {
            return true;
        }
    }

    throw std::runtime_error("Could not add value to tree at point " + (std::string) point);
}

template<class T> void Quadtree<T>::node::query(const cv::Rect &range, Values &result) const {
    if ((bounds & range).area() == 0) {
        return;
    }

    for (ListIteratorConst<Value> i(values); i.more(); i.next()) {
        const cv::Point &point = i->first;
        if (point.inside(range)) {
            result.append(*i);
        }
    }

    for (ListIterator<node> i(children); i.more();) {
        nodeP &child = i.next();
        child->query(range, result);
    }
}

template<class T> Quadtree<T>::Quadtree(const cv::Size &size):
    root(cv::Rect(0, 0, size.width, size.height))
{
    // Nothing to do.
}

template<class T> void Quadtree<T>::add(int x, int y, const T&value) {
    add(cv::Point(x, y), value);
}

template<class T> void Quadtree<T>::add(const cv::Point &point, const T &value) {
    root.add(point, value);
}

template<class T> typename Quadtree<T>::Values Quadtree<T>::query(const cv::Rect &range) const {
    Values result;
    root.query(range, result);
    return result;
}

template<class T> const cv::Size &Quadtree<T>::size() const {
    return root.bounds.size();
}

#endif
