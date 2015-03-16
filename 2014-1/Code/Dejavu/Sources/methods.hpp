#ifndef DEJAVU_METHODS_HPP
#define DEJAVU_METHODS_HPP

#include <clarus/clarus.hpp>

#include <opencv2/opencv.hpp>

#include <string>

void plot_eye_buffer(
    const std::string &folder,
    const std::string &run,
    const std::string &ext,
    int size
);

void plot_grayscale(
    const std::string &folder,
    const std::string &run,
    const std::string &ext,
    int size
);

void eye_buffer_show(const std::string &folder, const std::string &ext);

#endif
