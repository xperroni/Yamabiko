/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Dejavu.

Dejavu is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dejavu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dejavu. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cight/video_stream.hpp>
using cight::VideoStream;

#include <clarus/core/list.hpp>
using clarus::List;

#include <clarus/core/math.hpp>
#include <clarus/core/types.hpp>
#include <clarus/io/viewer.hpp>

#include <opencv2/opencv.hpp>

#include <fstream>
#include <iostream>
#include <string>

static const int KEY_0 = 1048624;
static const int KEY_LEFT = 1113937;
static const int KEY_RIGHT = 1113939;
static const int KEY_UP = 1113938;
static const int KEY_DOWN = 1113940;
static const int KEY_ENTER = 1048586;
static const int KEY_ESC = 1048603;

static cv::Mat teachFrame(List<cv::Mat> &frames, int index, VideoStream &stream) {
    while (frames.size() <= index) {
        cv::Mat frame = stream();
        if (frame.empty()) {
            return frames.last();
        }

        frames.append(frame);
    }

    return frames[index];
}

static cv::Mat merge(const cv::Mat &teach, const cv::Mat &replay, int shift) {
    cv::Mat shifted = teach.clone();
    if (shift != 0) {
        clarus::shift(shifted, 0, shift);

        int cols = shifted.cols;
        int x = (shift < 0 ? cols + shift : 0);
        int w = abs(shift);

        cv::Rect roi(x, 0, w, shifted.rows);
        cv::Mat blank(shifted, roi);
        cv::Mat paste(replay, roi);
        paste.copyTo(blank);
    }

    cv::Mat merged;
    cv::addWeighted(shifted, 0.5, replay, 0.5, 0.0, merged);
    return merged;
}

void cupido_run(const std::string &teachPath, const std::string &replayPath, int skip1, int skip2) {
    VideoStream teachStream(teachPath, skip1);
    List<cv::Mat> taught;
    int index = 0;

    int skip3 = skip1 * skip2;
    std::ofstream out("matches.txt");
    VideoStream replayStream(replayPath, skip3);
    for (int i = 0;; i += skip3) {
        cv::Mat replay = replayStream();
        if (replay.empty()) {
            break;
        }

        int shift = 0;
        for (;;) {
            cv::Mat teach = teachFrame(taught, index, teachStream);
            cv::Mat merged = merge(teach, replay, shift);
            viewer::show("Cupido", merged);

            int key = cv::waitKey();
            if (key == KEY_ESC) {
                return;
            }
            else if (key == KEY_ENTER) {
                List<int> match = (List<int>(), i, index * skip1, shift);
                std::cout << match << std::endl;
                out << match << std::endl;
                index += skip2;
                break;
            }
            else if (key == KEY_LEFT) {
                shift--;
            }
            else if (key == KEY_RIGHT) {
                shift++;
            }
            else if (key == KEY_0) {
                shift = 0;
            }
            else if (key == KEY_UP) {
                index++;
            }
            else if (key == KEY_DOWN) {
                if (index > 0) {
                    index--;
                }
                else {
                    std::cerr << "First teach frame reached." << std::endl;
                }
            }
            else {
                std::cerr << "Unknown key: " << key << std::endl;
            }
        }
    }
}

int cupido_main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "cupido <teach stream file> <replay stream file> <teach skip> <replay skip>" << std::endl;
        return -1;
    }

    cupido_run(argv[1], argv[2], types::from_string<int>(argv[3]), types::from_string<int>(argv[4]));

    return 0;
}

int main(int argc, char *argv[]) {
    return cupido_main(argc, argv);
}
