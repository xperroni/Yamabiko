#include <cight/drift_recorder.hpp>
using cight::Recorder;

#include <clarus/model/point.hpp>
using clarus::distance;
using clarus::Point;

#include<clarus/vision/images.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

#include <boost/regex.hpp>

#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

template<class T> static std::string folder_regexp(T value) {
    std::ostringstream out;
    out << "0?(\\d+)\\-"
        << std::setfill('0') << std::setw(2) << value
        << "$";

    return out.str();
}

static int folder_no(const std::string &root, const std::string &prefix, int series) {
    std::string e = folder_regexp(series);
    boost::regex NUMBER(e);

    int no = 1;
    for(fs::directory_iterator i(root), n; i != n; ++i) {
        const fs::path &path = *i;
        if (!fs::is_directory(path)) {
            continue;
        }

        std::string name = path.stem().native();
        if (name.find(prefix) != 0) {
            continue;
        }

        boost::smatch matches;
        if (!boost::regex_search(name, matches, NUMBER)) {
            continue;
        }

        int n2 = types::from_string<int>(matches[1].str());
        if (n2 >= no) {
            no = n2 + 1;
        }
    }

    return no;
}

static std::string folder_name(const std::string &root, const std::string &tag, int series) {
    time_t rawtime;
    time(&rawtime);

    struct tm *timeinfo = localtime(&rawtime);

    std::ostringstream name;
    name << std::setfill('0')
        << (1900 + timeinfo->tm_year) << '-'
        << std::setw(2) << (1 + timeinfo->tm_mon) << '-'
        << std::setw(2) << timeinfo->tm_mday << '-'
        << tag << '-';

    int no = folder_no(root, name.str(), series);
    name << std::setw(2) << no << '-' << std::setw(2) << series;

    std::string folder = root + '/' + name.str();
    int status = system(("mkdir -p " + folder).c_str());
    if (status != 0) {
        throw std::runtime_error(
            "Could not create output folder (error no. " + types::to_string(status) + ")"
        );
    }

    return folder;
}

Recorder::Recorder(
    const std::string &_root,
    const std::string &_prefix,
    const std::string &_tag,
    int _series
):
    root(_root),
    prefix(_prefix),
    tag(_tag),
    series(_series),
    folder(folder_name(root, tag, series)),
    odometry(new std::ofstream((folder + "/odometry.txt").c_str())),
    d(0.0)
{
    // Nothing to do.
}

Recorder::~Recorder() {
    // Nothing to do.
}

void Recorder::operator () (const cv::Mat &image, const Point &p) {
    double dt = p[0];
    if (dt == d) {
        return;
    }

    std::string label = types::to_string(clock());
    images::save(image, folder, prefix + label, "png");
    *odometry << p << std::endl;
    d = dt;
}

void Recorder::clip() {
    series++;
    folder = folder_name(root, tag, series);

    odometry->close();
    odometry.reset(new std::ofstream((folder + "/odometry.txt").c_str()));
}

double Recorder::travelled() const {
    return d;
}
