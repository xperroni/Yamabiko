#include <clarus/vgram/layer_threaded.hpp>
using vgram::Control;
using vgram::LayerThreaded;

#include <clarus/vgram/bitstring.hpp>
using vgram::Bitstring;

#include <clarus/core/list.hpp>
using clarus::List;

#include <boost/bind.hpp>

LayerThreaded::LayerThreaded(int _cores, int width, int height, int bytes):
    Layer(width, height, bytes),
    cores(_cores),
    dispatcher(),
    idler(dispatcher),
    threads(),
    barrier(std::min(cores, height) + 1)
{
    for (int i = 0; i < cores; i++) {
        threads.create_thread(boost::bind(&boost::asio::io_service::run, &dispatcher));
    }
}

LayerThreaded::~LayerThreaded() {
    dispatcher.stop();
    threads.join_all();
}

inline int bigger(uint32_t dz, uint32_t *dk, int n) {
    int w = 0;
    uint32_t dw = *dk;
    for (int k = 1; k < n; k++) {
        if (dw < *(++dk)) {
            dw = *dk;
            w = k;
        }
    }

    return (dz < dw ? w : n);
}

static void retrieve_job(
    LayerThreaded &layer, int r0, int rn,
    const cv::Mat &input, Control &control, int top,
    List<cv::Mat> &response
) {
    const List<cv::Mat> &inputs = layer.inputs;
    const List<cv::Mat> &outputs = layer.outputs;
    cv::Mat &o = response[0];
    cv::Mat &d = response[1];
    cv::Mat &l = response[2];

    control.resize(0, r0, rn);
    for (; control.more(); control.next()) {
        size_t i = control.index(0);
        size_t j = control.index(1);
        size_t z = control.index(2);
        if (z >= inputs.size()) {
            continue;
        }

        uint32_t dz = Bitstring::distance(input, inputs[z], i, j);
        uint32_t *dk = &(d.at<uint32_t>(i, j, 0));
        int w = bigger(dz, dk, top);
        if (w < top) {
            o.at<uint8_t>(i, j, w) = outputs[z].at<uint8_t>(i, j);
            d.at<uint32_t>(i, j, w) = dz;
            l.at<int32_t>(i, j, w) = z;
        }
    }
    layer.barrier.wait();
}

List<cv::Mat> LayerThreaded::retrieve(const cv::Mat &input, Control &control, int top) {
    int rows = size[0];
    int cols = size[1];
    const int sizes[] = {rows, cols, top};

    cv::Mat o(3, sizes, CV_8U, cv::Scalar(0));
    cv::Mat d(3, sizes, CV_32S, cv::Scalar(8 * size[2]));
    cv::Mat l(3, sizes, CV_32S, cv::Scalar(0));

    List<cv::Mat> response;
    response.append(o);
    response.append(d);
    response.append(l);

    int n = std::min(rows, cores);
    int slice = rows / n;
    int last = n - 1;

    for (int i = 0, r0 = 0, rn = slice; i < n; i++, r0 += slice, rn += slice) {
        if (i == last) {
            rn = rows;
        }

        dispatcher.post(
            boost::bind(
                retrieve_job, boost::ref(*this), r0, rn,
                boost::ref(input), boost::ref(control), top,
                boost::ref(response)
            )
        );
    }

    barrier.wait();

    return response;
}
