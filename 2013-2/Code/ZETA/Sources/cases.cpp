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

#include "cases.hpp"

void dither_image(const std::string &folder, const std::string &name, const std::string &ext) {
    cv::Mat bgr = cv::imread(folder + "/" + name + "." + ext);
    std::vector<cv::Mat> channels;
    cv::split(bgr, channels);

    cv::Mat hls;
    cv::cvtColor(bgr, hls, CV_BGR2HLS);
    std::vector<cv::Mat> hls_channels;
    cv::split(hls, hls_channels);
    channels.push_back(hls_channels[1]);

    const char *names[] = {"b", "g", "r", "l"};
    for (int i = 0; i < 4; i++) {
        cv::Mat &channel = channels[i];
        cv::Mat dithered = dither::transform(channel, 1);
        cv::imwrite(folder + "/" + name + "_" + names[i] + ".png", channel);
        cv::imwrite(folder + "/" + name + "_" + names[i] + "_dither.png", dithered);
    }
}

void depth_map_plot(
    const std::string &folder,
    const std::string &name,
    const std::string &ext,
    const cv::Size &size,
    const cv::Size &scale
) {
    cv::Mat depths = depths::load(folder + "/" + name + ".dat", size);
    cv::Mat plot = depths::bgr(depths, scale);
    cv::imwrite(folder + "/" + name + "." + ext, plot);
}

void disassemble_sample(const std::string &folder, const std::string &name, const std::string &ext) {
    cv::Mat bgr = cv::imread(folder + "/" + name + "." + ext);
    const cv::Size &size = bgr.size();

    cv::Mat hls;
    std::vector<cv::Mat> channels;
    cv::cvtColor(bgr, hls, CV_BGR2HLS);
    cv::split(hls, channels);

    cv::Mat h = channels[0];
    cv::Mat l = channels[1];
    cv::Mat s = channels[2];
    cv::Mat e = sobel::filter(l);

    cv::imwrite(folder + "/" + name + "_h." + ext, h);
    cv::imwrite(folder + "/" + name + "_l." + ext, l);
    cv::imwrite(folder + "/" + name + "_s." + ext, s);
    cv::imwrite(folder + "/" + name + "_e." + ext, e);

    clarus::sampler sampler_h(64, clarus::sampler::gaussian(4, size));
    clarus::sampler sampler_l(64, clarus::sampler::gaussian(4, size));
    clarus::sampler sampler_s(64, clarus::sampler::gaussian(4, size));
    clarus::sampler sampler_e(64, clarus::sampler::gaussian(4, size));

    cv::Mat s_h = sampler_h.sample2d<uchar>(h);
    cv::Mat s_l = sampler_l.sample2d<uchar>(l);
    cv::Mat s_s = sampler_s.sample2d<uchar>(s);
    cv::Mat s_e = sampler_e.sample2d<uchar>(e);

    cv::imwrite(folder + "/" + name + "_h_s." + ext, s_h);
    cv::imwrite(folder + "/" + name + "_l_s." + ext, s_l);
    cv::imwrite(folder + "/" + name + "_s_s." + ext, s_s);
    cv::imwrite(folder + "/" + name + "_e_s." + ext, s_e);

    cv::Mat m_h = sampler_h.minchinton2bgr<uchar>(h);
    cv::Mat m_l = sampler_l.minchinton2bgr<uchar>(l);
    cv::Mat m_s = sampler_s.minchinton2bgr<uchar>(s);
    cv::Mat m_e = sampler_e.minchinton2bgr<uchar>(e);

    cv::imwrite(folder + "/" + name + "_h_m." + ext, m_h);
    cv::imwrite(folder + "/" + name + "_l_m." + ext, m_l);
    cv::imwrite(folder + "/" + name + "_s_m." + ext, m_s);
    cv::imwrite(folder + "/" + name + "_e_m." + ext, m_e);
}

void train_and_test(const std::string &train, const std::string &test) {
    zeta::ZETA<256> z(100, 4.0, cv::Size(1704, 2272), cv::Size(86, 107));
    z.train(train);
    z.test(test);
}
