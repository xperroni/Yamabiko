#include <cight/image_stream.hpp>
using cight::ImageStream;

#include <clarus/vision/images.hpp>
#include <clarus/core/types.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

ImageStream::ImageStream(const std::string &folder) {
    for(fs::directory_iterator i(folder), n; i != n; ++i) {
        const fs::path &file = *i;
        if (file.extension() != ".png") {
            continue;
        }

        std::string name = file.stem().native();
        if (name.find_first_of("still") != 0) {
            continue;
        }

        long id = types::from_string<long>(name.substr(name.find_first_of('-') + 1));
        files[id] = file.native();
    }

    next = files.begin();
    done = files.end();
}

ImageStream::~ImageStream() {
    // Nothing to do.
}

cv::Mat ImageStream::operator () () {
    cv::Mat image = images::load(next->second);
    next++;
    return image;
}

bool ImageStream::more() const {
    return (next != done);
}

long ImageStream::current() const {
    return next->first;
}

long ImageStream::departure() const {
    return files.begin()->first;
}

long ImageStream::elapsed() const {
    return current() - departure();
}
