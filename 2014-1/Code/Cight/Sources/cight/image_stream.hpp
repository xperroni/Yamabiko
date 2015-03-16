#ifndef DEJAVU_IMAGE_STREAM_HPP
#define DEJAVU_IMAGE_STREAM_HPP

#include <opencv2/opencv.hpp>

#include <map>
#include <string>

namespace cight {
    class ImageStream;
}

class cight::ImageStream {
    std::map<long, std::string> files;

    std::map<long, std::string>::const_iterator next;

    std::map<long, std::string>::const_iterator done;

public:
    /*
    Creates a new image stream from the given folder.
    */
    ImageStream(const std::string &folder);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~ImageStream();

    /*
    Returns the next image in the sequence.
    */
    cv::Mat operator () ();

    /*
    Returns whether there are any more images in the sequence.
    */
    bool more() const;

    /*
    Returns the time tag of the current record.
    */
    long current() const;

    /*
    Returns the time tag of the first record.
    */
    long departure() const;

    /*
    Returns the time difference from the first to the current record.
    */
    long elapsed() const;
};

#endif // DEJAVU_IMAGE_STREAM_HPP
