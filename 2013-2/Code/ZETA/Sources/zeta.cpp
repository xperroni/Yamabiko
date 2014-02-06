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

int main(int argc, char *argv[]) {
/*    dither_image(
        "/home/helio/Documents/Yamabiko/2013-2/Weightless Neural Networks for Depth estimation/References",
        "statroad4-p-77t0",
        "jpg"
    );

    depth_map_plot(
        "/home/helio/Documents/Yamabiko/2013-2/Weightless Neural Networks for Depth estimation/References",
        "statroad2-p-282t0",
        "png",
        cv::Size(86, 107),
        cv::Size(1704, 2272)
    );
*/
//    disassemble_sample("/home/helio/Documents", "patch", "png");
    train_and_test(argv[1], argv[2]);
/*    train_and_test(
        "/home/helio/Projects/Data/Saxena/once.txt",
        "/home/helio/Projects/Data/Saxena/once.txt"
    );
*/
    return 0;
}
