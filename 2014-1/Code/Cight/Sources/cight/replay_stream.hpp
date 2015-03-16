#ifndef DEJAVU_REPLAY_STREAM_HPP
#define DEJAVU_REPLAY_STREAM_HPP

#include <cight/image_stream.hpp>
#include <cight/sensor_stream.hpp>

#include <fstream>
#include <string>

namespace cight {
    class ReplayStream;
}

class cight::ReplayStream: public SensorStream {
    ImageStream images;

    std::ifstream odometry;

public:
    /*
    Creates a new sensor stream replay from data in the given folder.
    */
    ReplayStream(const std::string &folder);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ReplayStream();

    /* See SensorStream::camera */
    virtual S operator () ();

    /* See SensorStream::active */
    virtual bool active();
};

#endif
