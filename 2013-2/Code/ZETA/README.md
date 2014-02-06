ZETA
====

The horiZontal slidEr esTimator Architecture (ZETA) is a [VG-RAM Weightless Neural Network](http://www.researchgate.net/publication/220000031_VG-RAM_WNN_Approach_to_Monocular_Depth_Perception?ev=prf_pub) system for estimating depth maps. It is developed in C++ 03 on top of [boost](http://www.boost.org/), [OpenCV](http://opencv.org/) and [Clarus](https://github.com/xperroni/Clarus).

ZETA is a purely experimental project, and serves no practical purpose other than demonstrating the use of the Clarus library, particularly its VG-RAM API.

ZETA is developed inside the [Code::Blocks](http://www.codeblocks.org/) IDE, and currently this is the only supported build environment. I intend to provide a set of [autotools](http://www.gnu.org/savannah-checkouts/gnu/automake/manual/html_node/index.html#Top) files for cross-platform building in the future.

TO DO
-----

* Provide a set of autotools files for building the library;
* Document the source files;
* Move the code base to C++ 11, when support is widespread enough.

