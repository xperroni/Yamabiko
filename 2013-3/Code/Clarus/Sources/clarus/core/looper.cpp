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

#include <boost/bind.hpp>

Looper::Loop::Loop(Body body, Looper &looper):
    running(true)
{
    thread.reset(
        new boost::thread(
            &Loop::run, this, body, boost::ref(looper)
        )
    );
}

void Looper::Loop::run(Body body, Looper &looper) {
    body(looper, running);
    running = false;
}

Looper::Loop &Looper::peek() {
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

void Looper::wipe() {
    locked_ptr<Loops> loops = locker.read(this->loops);
    for (Loops::iterator i = loops->begin(), n = loops->end(); i != n; ++i) {
        LoopP &loop = (*i);
        loop->running = false;
    }
}

void Looper::start(Body body) {
    LoopP loop(new Loop(body, *this));
    locked_ptr<Loops> loops = locker.write(this->loops);
    loops->push_back(loop);
}

void Looper::stop(bool blocking) {
    wipe();
    if (blocking) {
        join();
    }
}

void Looper::join() {
    while (size() > 0) {
        Loop &loop = peek();
        loop.thread->join();
        pop();
    }
}

bool Looper::has(const std::string &name) {
    return locker.read(table)->has(name);
}
