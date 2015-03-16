/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_CORE_LOOPER_HPP
#define CLARUS_CORE_LOOPER_HPP

#include <clarus/core/context.hpp>
#include <clarus/core/loop.hpp>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace clarus {
    class Looper;
}

class clarus::Looper: public Context {
    typedef std::deque<Loop::P> Loops;

    Loops loops;

    Loop &peek();

    void pop();

    size_t size();

public:
    typedef boost::function<void(Looper &looper, Loop &loop)> F;

    /*
    Default constructor.
    */
    Looper();

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Looper();

    Loop::P start(F body);

    void stop(bool blocking = false);

    void join();
};

#endif
