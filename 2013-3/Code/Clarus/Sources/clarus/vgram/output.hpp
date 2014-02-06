#ifndef CLARUS_VGRAM_OUTPUT_HPP
#define CLARUS_VGRAM_OUTPUT_HPP

#include <boost/shared_ptr.hpp>

#include <limits>

namespace vgram {
    template<class T> class output;
}

template<class T> class vgram::output {
    boost::shared_ptr<T> t;

    size_t d;

public:
    output();

    output(boost::shared_ptr<T> t, size_t d);

    output(const T &t, size_t d);

    bool empty() const;

    const T &value() const;

    size_t error() const;
};

template<class T> vgram::output<T>::output():
    t(),
    d(std::numeric_limits<size_t>::max())
{
    // Nothing to do.
}

template<class T> vgram::output<T>::output(boost::shared_ptr<T> _t, size_t _d):
    t(_t),
    d(_d)
{
    // Nothing to do.
}

template<class T> vgram::output<T>::output(const T &_t, size_t _d):
    t(new T(_t)),
    d(_d)
{
    // Nothing to do.
}

template<class T> bool vgram::output<T>::empty() const {
    return (t.get() == NULL);
}

template<class T> const T &vgram::output<T>::value() const {
    return *t;
}

template<class T> size_t vgram::output<T>::error() const {
    return d;
}

#endif
