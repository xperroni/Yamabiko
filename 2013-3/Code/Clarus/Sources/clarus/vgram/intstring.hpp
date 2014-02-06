#ifndef CLARUS_VGRAM_NUMBER_HPP
#define CLARUS_VGRAM_NUMBER_HPP

#include <clarus/core/list.hpp>

namespace vgram {
    class intstring;
}

class vgram::intstring: public List<long> {
public:
    intstring();

    intstring(size_t size);

    size_t distance(const intstring &that) const;
};

#endif
