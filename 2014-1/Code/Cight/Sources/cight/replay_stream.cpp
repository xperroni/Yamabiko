#include <cight/replay_stream.hpp>
using cight::ReplayStream;

#include <clarus/model/point.hpp>
using clarus::Point;

ReplayStream::ReplayStream(const std::string &folder):
    images(folder),
    odometry((folder + "/odometry.txt").c_str())
{
    // Nothing to do.
}

ReplayStream::~ReplayStream() {
    // Nothing to do.
}

bool ReplayStream::active() {
    return images.more();
}

ReplayStream::S ReplayStream::operator () () {
    Point p;
    odometry >> p;
    return S(images(), p[0]);
}
