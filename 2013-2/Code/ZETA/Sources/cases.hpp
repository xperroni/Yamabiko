/*
This file is part of ZETA.

ZETA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ZETA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ZETA.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ZETA_CASES_HPP
#define ZETA_CASES_HPP

#include "zeta.hpp"

#include <string>

void depth_map_plot(
    const std::string &folder,
    const std::string &name,
    const std::string &ext,
    const cv::Size &size,
    const cv::Size &scale
);

void dither_image(const std::string &folder, const std::string &name, const std::string &ext);

void disassemble_sample(const std::string &folder, const std::string &name, const std::string &ext);

void train_and_test(const std::string &train, const std::string &test);

#endif
