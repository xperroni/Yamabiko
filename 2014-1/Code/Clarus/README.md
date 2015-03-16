Clarus
======

Clarus is a library for machine learning, geared towards computer vision problems. It is developed in C++ 03 on top of [boost](http://www.boost.org/) and [OpenCV](http://opencv.org/). It is divided in several modules:

* `core`: multi-threading facilities, the `Bag` heterogeneous collection, and some other conveniences;
* `model`: classes and algorithms dealing with high-level (usually geometric) models and concepts;
* `io`: facilities for interfacing with sensors (such as cameras) and displaying data;
* `vgram`: an API for working with [VG-RAM Weightless Neural Networks](https://github.com/xperroni/Yamabiko/tree/master/2013-2);
* `vision`: operations on images.

Clarus is developed inside the [Code::Blocks](http://www.codeblocks.org/) IDE, and currently this is the only supported build environment. I intend to provide a set of [autotools](http://www.gnu.org/savannah-checkouts/gnu/automake/manual/html_node/index.html#Top) files for cross-platform building in the future.

TO DO
-----

* Provide a set of autotools files for building the library;
* Document the API and source files;
* Move the code base to C++ 11, when support is widespread enough.

Trivia
------

The library's name comes from the [Phidippus clarus spider](http://en.wikipedia.org/wiki/Phidippus_clarus), which was found to have a [huge brain](http://news.nationalgeographic.com/news/2011/12/111219-spiders-big-brains-bodies-legs-webs-animals-science/) relative to its body size. It is my hope that, like the spider, Clarus the library won't look like much at first sight, but reveal a deeper side over time.
