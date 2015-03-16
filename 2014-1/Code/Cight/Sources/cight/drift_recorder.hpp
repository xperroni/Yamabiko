#ifndef CIGHT_RECORDER_HPP
#define CIGHT_RECORDER_HPP

#include <clarus/model/point.hpp>

#include <boost/smart_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <string>

namespace cight {
    class Recorder;
}

class cight::Recorder {
    /* Parent folder where image sequences are stored. */
    std::string root;

    /* Prefix used to name the recorded images. */
    std::string prefix;

    /* Tag for the image sequence. */
    std::string tag;

    /* Image series number. */
    int series;

    /* Folder where data will be stored. */
    std::string folder;

    /* Output file to which odometry data will be written. */
    boost::shared_ptr<std::ofstream> odometry;

    /* Distance traveled by the robot. */
    double d;
public:
    /*
    Creates a new recorder. Data will be written to a new subfolder under the given
    root folder, and its name will include the given tag.
    */
    Recorder(
        const std::string &root,
        const std::string &prefix,
        const std::string &tag,
        int series
    );

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Recorder();

    /*
    Record the given image and position.
    */
    void operator () (const cv::Mat &image, const clarus::Point &p);

    /*
    Starts a new series within the current recording session.
    */
    void clip();

    /*
    Returns the distance travelled by the robot.
    */
    double travelled() const;
};

#endif // CIGHT_RECORDER_HPP
