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

#ifndef ZETA_HPP
#define ZETA_HPP

#include "domain.hpp"
#include "parameters.hpp"
#include "output.hpp"

#include "assembler_top10.hpp"
#include "extractor.hpp"
#include "neurons.hpp"
#include "sampler_dither.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <string>

namespace zeta {
    namespace fs = boost::filesystem;

    template<size_t r> class ZETA;
}

template<size_t r> class zeta::ZETA {
    struct Z {
        typedef zeta::X X;

        typedef zeta::Y Y;

        typedef vgram::bitstring<r> B;

        typedef zeta::F F;

        typedef zeta::G G;

        typedef zeta::J J;

        typedef zeta::L L;

        typedef zeta::Neurons<B> N;

        typedef zeta::Output O;

        typedef zeta::Parameters P;

        typedef zeta::DitherSampler<B> alpha;

        typedef zeta::Extractor beta;

        typedef zeta::Neurons<B> gamma;

        typedef zeta::AssemblerTop10 omega;
    };

    vgram::network<Z> w;

    cv::Size images_size;

    cv::Size depths_size;

public:
    ZETA(int samples, double sigma, const cv::Size &images, const cv::Size &depths);

    void train(const std::string &path);

    void test(const std::string &path);
};


template<size_t r> zeta::ZETA<r>::ZETA(
    int samples,
    double sigma,
    const cv::Size &images,
    const cv::Size &depths
):
    w(typename Z::P(r, samples, sigma, images, depths)),
    images_size(images),
    depths_size(depths)
{
    // Nothing to do.
}

template<size_t r> void zeta::ZETA<r>::train(const std::string &path) {
    std::cout << "Training..." << std::endl;

    fs::path cases(path);
    fs::path folder(cases.parent_path());
    fs::ifstream file(cases);
    for (std::string name; std::getline(file, name) && name != "";) {
        std::cout << "    " << name << std::endl;
        std::string image_path = (folder / (name + ".jpg")).native();
        std::string depths_path = (folder / (name + ".dat")).native();

        cv::Mat image = cv::imread(image_path);
        cv::Mat depths = depths::load(depths_path, depths_size);
        w.set(image, depths);
    }
}

template<size_t r> void zeta::ZETA<r>::test(const std::string &path) {
    std::cout << "Testing..." << std::endl;

    fs::path cases(path);
    fs::path folder(cases.parent_path());
    fs::ifstream file(cases);
    for (std::string name; std::getline(file, name) && name != "";) {
        std::cout << "    " << name << std::endl;
        std::string image_path = (folder / (name + ".jpg")).native();

        cv::Mat image = cv::imread(image_path);
        cv::Mat depths = w.get(image).output();

        std::string depths_path = (folder / (name + ".test.dat")).native();
        depths::save(depths_path, depths);

        std::string depths_jpg_path = (folder / (name + ".test.jpg")).native();
        cv::imwrite(depths_jpg_path, depths::bgr(depths, images_size));

        std::string refer_path = (folder / (name + ".dat")).native();
        cv::Mat refer = depths::load(refer_path, depths_size);

        std::string refer_jpg_path = (folder / (name + ".dat.jpg")).native();
        cv::imwrite(refer_jpg_path, depths::bgr(refer, images_size));
    }
}

#endif
