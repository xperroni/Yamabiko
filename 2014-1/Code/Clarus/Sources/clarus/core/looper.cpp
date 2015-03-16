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

#include <clarus/core/looper.hpp>
using clarus::Loop;
using clarus::Looper;

#include <boost/bind.hpp>

Looper::Looper():
    Context()
{
    // Nothing to do.
}

Looper::~Looper() {
    // Nothing to do.
}

Loop &Looper::peek() {
    locked_ptr<Loops> loops = locker.read(this->loops);
    return *(loops->front());
}

void Looper::pop() {
    locked_ptr<Loops> loops = locker.write(this->loops);
    loops->pop_front();
}

size_t Looper::size() {
    locked_ptr<Loops> loops = locker.read(this->loops);
    return loops->size();
}

Loop::P Looper::start(F body) {
    Loop::P loop(
        new Loop(
            boost::bind(body, boost::ref(*this), _1),
            this
        )
    );

    locked_ptr<Loops> loops = locker.write(this->loops);
    loops->push_back(loop);
    return loop;
}

void Looper::stop(bool blocking) {
    while (size() > 0) {
        Loop &loop = peek();
        loop.stop(blocking);
        pop();
    }
}

void Looper::join() {
    while (size() > 0) {
        Loop &loop = peek();
        loop.join();
        pop();
    }
}
