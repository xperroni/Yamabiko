#include <clarus/vgram/intstring.hpp>

#include <clarus/core/types.hpp>

#include <stdexcept>

vgram::intstring::intstring():
    List<long>()
{
    // Nothing to do.
}

vgram::intstring::intstring(size_t size):
    List<long>(size)
{
    // Nothing to do.
}

size_t vgram::intstring::distance(const intstring &that) const {
    if (size() != that.size()) {
        throw std::runtime_error(
            "List sizes differ: " + types::to_string(size()) + " != " + types::to_string(that.size())
        );
    }

    size_t d = 0;
    for (ListIteratorConst<long> i(*this), j(that); i.more();) {
        long a = i.next();
        long b = j.next();
        d += labs(a - b);
    }

    return d;
}
