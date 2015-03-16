#ifndef CLARUS_VGRAM_LAYER_THREADED_HPP
#define CLARUS_VGRAM_LAYER_THREADED_HPP

#include <clarus/vgram/layer.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

namespace vgram {
    struct LayerThreaded;
}

struct vgram::LayerThreaded: public Layer {
    // Number of cores available on the machine.
    const int cores;

    // Asynchronous IO service to dispatch work to threads on the pool.
    boost::asio::io_service dispatcher;

    // Work object that keeps the IO service running.
    boost::asio::io_service::work idler;

    // Thread pool.
    boost::thread_group threads;

    // Barrier used to coordinate work among threads.
    boost::barrier barrier;

    /*
    Creates a new layer employing the given number of threads, of given width and height,
    storing bit strings of the given length.
    */
    LayerThreaded(int cores, int width, int height, int bytes);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~LayerThreaded();

    // See layer.hpp
    virtual clarus::List<cv::Mat> retrieve(const cv::Mat &input, Control &control, int top = 1);
};

#endif // CLARUS_VGRAM_LAYER_THREADED_HPP
