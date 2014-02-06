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

#include "navigator.hpp"

#include "difference.hpp"
#include "scene.hpp"

#include <limits>

static const int KEY_LEFT = 65361;
static const int KEY_RIGHT = 65363;
static const int KEY_UP = 65362;
static const int KEY_DOWN = 65364;
static const int KEY_S = (int) 's';
static const int KEY_X = (int) 'x';
static const int KEY_Z = (int) 'z';
static const int KEY_ESC = 27;

static dejavu::Mode advance(int key, int i, int &j, int &k) {
    static dejavu::Parameters p;
    static dejavu::FORWARD forward(p);
    static dejavu::BACKWARD backward(p);

    int change = (key == KEY_UP ? 1 : -1);
    int direction = (i % 2 == 0 ? 1 : -1);
    int value = j + change * direction;
    if (value < 0 || value > 9) {
        return dejavu::UNKNOWN;
    }

    j = value;
    k = 12 - k;

    if (key == KEY_UP) {
        return forward;
    }
    else {
        return backward;
    }
}

static dejavu::Mode slide(int key, int &i, int &j, int &k) {
    int change = (key == KEY_LEFT ? 1 : -1);
    int value = i + change;
    if (value < 0 || value > 11) {
        return dejavu::UNKNOWN;
    }

    i = value;
    j = 9 - j;
    k = 12 - k;

    return (key == KEY_LEFT ? dejavu::SIDEWAYS_LEFT : dejavu::SIDEWAYS_RIGHT);
}

static dejavu::Mode turn(int key, int i, int j, int &k) {
    int change = (key == KEY_X ? 1 : -1);
    int direction = (j % 2 == 0 ? 1 : -1);
    int value = k + change * direction;
    if (value < 0 || value > 12) {
        return dejavu::UNKNOWN;
    }

    k = value;

    return (key == KEY_X ? dejavu::TURNING_RIGHT : dejavu::TURNING_LEFT);
}

cv::Mat concatenate(const cv::Mat &img1, const cv::Mat &img2) {
    cv::Size size(img1.cols + img2.cols, std::max(img1.rows, img2.rows));
    cv::Mat concatenated(size, img1.type(), cv::Scalar::all(127));
    cv::Mat side1(concatenated, cv::Rect(0, 0, img1.cols, img1.rows));
    cv::Mat side2(concatenated, cv::Rect(img1.cols, 0, img2.cols, img2.rows));
    img1.copyTo(side1);
    img2.copyTo(side2);
    return concatenated;
}

cv::Mat equalize(const cv::Mat &image) {
    CHANNEL_WISE(equalize, image);

    cv::Mat equalized;
    cv::normalize(image, equalized, 0, 255, CV_MINMAX);
    return equalized;
}

void conormalize(const cv::Mat &img1, const cv::Mat &img2, cv::Mat &norm1, cv::Mat &norm2) {
    cv::Mat two = equalize(concatenate(img1, img2));
    cv::Mat side1(two, cv::Rect(0, 0, img1.cols, img1.rows));
    cv::Mat side2(two, cv::Rect(img1.cols, 0, img2.cols, img2.rows));
    side1.copyTo(norm1);
    side2.copyTo(norm2);
}

void draw_displacement(const cv::Mat &bgr1, const cv::Mat &bgr2, dejavu::Mode mode) {
    static cv::Scalar YELLOW(0, 255, 255);
/*
    cv::Mat bgr1, bgr2;
    conormalize(img1, img2, bgr1, bgr2);
*/
    dejavu::Scene scene1(bgr1);
    dejavu::Scene scene2(bgr2);

    cv::Mat overlay1 = bgr1.clone();
    cv::Mat overlay2 = bgr2.clone();

    dejavu::Difference d(dejavu::Position(0, 0, 0), scene1, scene2, mode);
    for (int i = 0, n = d.size(); i < n; i++) {
        const Assignment &a = d.match(i);
        const cv::Rect &roi1 = scene1.region(a.first);
        const cv::Rect &roi2 = scene2.region(a.second);
        std::string label = types::to_string(i);

        cv::rectangle(overlay1, roi1, YELLOW);
        cv::putText(overlay1, label, point(roi1), cv::FONT_HERSHEY_PLAIN, 1, YELLOW);

        cv::rectangle(overlay2, roi2, YELLOW);
        cv::putText(overlay2, label, point(roi2), cv::FONT_HERSHEY_PLAIN, 1, YELLOW);
    }

    viewer::show("Previous", overlay1, 50, 25);
    viewer::show("Current", overlay2, 650, 550);
}

cv::Mat load(const std::string &folder, int i, int j, int k) {
        std::string name =
            "still-" +
            types::to_string(i) + "-" +
            types::to_string(j) + "-" +
            types::to_string(k)
        ;

        return images::load(folder, name, "png");
}

static std::vector<boost::shared_ptr<dejavu::Scene> > scene_cache;

dejavu::Scene &load_forward(const std::string &folder, int i, int j, int k) {
    if (scene_cache.size() <= (size_t) i) {
        boost::shared_ptr<dejavu::Scene> scene(new dejavu::Scene(load(folder, i, j, k)));
        scene_cache.push_back(scene);
    }

    return *(scene_cache[i]);
}

dejavu::Scene &load_sideways(const std::string &folder, int i, int j, int k) {
    if (scene_cache.size() <= (size_t) j) {
        boost::shared_ptr<dejavu::Scene> scene(new dejavu::Scene(load(folder, i, j, k)));
        scene_cache.push_back(scene);
    }

    return *(scene_cache[j]);
}

dejavu::Scene &load_turning(const std::string &folder, int i, int j, int k) {
    if (scene_cache.size() <= (size_t) k) {
        boost::shared_ptr<dejavu::Scene> scene(new dejavu::Scene(load(folder, i, j, k)));
        scene_cache.push_back(scene);
    }

    return *(scene_cache[k]);
}

void purge_scene() {
    scene_cache.clear();
}

static void draw_mask(const cv::Mat &bgr) {
    cv::Mat channel = colors::saturation(bgr);
    cv::normalize(channel, channel, 0, 255, CV_MINMAX);

    cv::Mat pyr;
    cv::cvtColor(channel, pyr, CV_GRAY2BGR);
    cv::pyrMeanShiftFiltering(pyr.clone(), pyr, 15, 40);

    cv::Mat gray;
    cv::cvtColor(pyr, gray, CV_BGR2GRAY);

    cv::Mat edges;
    cv::Canny(gray, edges, 150, 150);
    gray -= edges;

    cv::normalize(gray, gray, 0, 255, CV_MINMAX);
    cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    cv::erode(gray.clone(), gray, cv::Mat());
    viewer::show("Mask", gray);
}

static std::string FOLDER = "/home/helio/Roboken/Data/Landmarks/2013-12-03-01";

void navigate() {
    int i = 0, j = 0, k = 5;
    cv::Mat s1 = load(FOLDER, i, j, k);
    dejavu::Mode mode = dejavu::TURNING_RIGHT;
    for (k = 6;;) {
        cv::Mat s2 = load(FOLDER, i, j, k);
        draw_displacement(s1, s2, mode);
        s1 = s2;

        int key = cv::waitKey();
        if (key == KEY_ESC) {
            return;
        }
        else if (key == KEY_DOWN || key == KEY_UP) {
            mode = advance(key, i, j, k);
        }
        else if (key == KEY_LEFT || key == KEY_RIGHT) {
            mode = slide(key, i, j, k);
        }
        else if (key == KEY_X || key == KEY_Z) {
            mode = turn(key, i, j, k);
        }
/*
        else if (key == KEY_S) {
            save(plot, i, j, k);
        }
*/
    }
}

static cv::Mat step(int index) {
    std::string name = "still-" + types::to_string(index);
    return images::load("/home/helio/Roboken/Data/Landmarks/2013-10-29-03", name, "png");
}

void back_and_forth() {
    for (int i = 0;;) {
        cv::Mat bgr = step(i);

        draw_mask(bgr);

        int key = cv::waitKey();
        if (key == KEY_ESC) {
            break;
        }
        else if (key == KEY_DOWN && i > 0) {
            i--;
        }
        else if (key == KEY_UP && i < 9) {
            i++;
        }
    }
}

void print_displacement_absolute(dejavu::Scene &scene1, dejavu::Scene &scene2) {
    dejavu::Difference d(dejavu::Position(0, 0, 0), scene1, scene2);
    for (int i = 0, n = d.size(); i < n; i++) {
        const Assignment &a = d.match(i);

        const cv::Rect &roi1 = scene1.region(a.first);
        const cv::Rect &roi2 = scene2.region(a.second);

        printf("(%d, %d, %d, %d) ", roi1.width, roi1.height, roi1.x, roi1.y);
        printf("(%d, %d, %d, %d) ", roi2.width, roi2.height, roi2.x, roi2.y);
        printf("%d\n", d.change(i).error());
    }

    printf("\n");
}

void statistics(const std::string &folder) {
    printf("Forward movements:\n");

    for (int v = 0; v < 10; v++) {
        for (int t = 0; t < 10; t++) {
            for (int u = 0; u < 10; u++) {
                for (int u2 = u + 1; u2 < 10; u2++) {
                    printf("(%2d, %2d, %2d) -> (%2d, %2d, %2d)\n", u, v, t, u2, v, t);
                    dejavu::Scene &scene1 = load_forward(folder, u, v, t);
                    dejavu::Scene &scene2 = load_forward(folder, u2, v, t);
                    print_displacement_absolute(scene1, scene2);
                }
            }

            purge_scene();
        }
    }

    printf("Sideways movements:\n");

    for (int u = 0; u < 10; u++) {
        for (int t = 0; t < 10; t++) {
            for (int v = 0; v < 10; v++) {
                for (int v2 = v + 1; v2 < 10; v2++) {
                    printf("(%2d, %2d, %2d) -> (%2d, %2d, %2d)\n", u, v, t, u, v2, t);
                    dejavu::Scene &scene1 = load_sideways(folder, u, v, t);
                    dejavu::Scene &scene2 = load_sideways(folder, u, v2, t);
                    print_displacement_absolute(scene1, scene2);
                }
            }

            purge_scene();
        }
    }

    printf("Turning movements:\n");

    for (int u = 0; u < 10; u++) {
        for (int v = 0; v < 10; v++) {
            for (int t = 0; t < 10; t++) {
                for (int t2 = t + 1; t2 < 10; t2++) {
                    printf("(%2d, %2d, %2d) -> (%2d, %2d, %2d)\n", u, v, t, u, v, t2);
                    dejavu::Scene &scene1 = load_turning(folder, u, v, t);
                    dejavu::Scene &scene2 = load_turning(folder, u, v, t2);
                    print_displacement_absolute(scene1, scene2);
                }
            }

            purge_scene();
        }
    }
}
