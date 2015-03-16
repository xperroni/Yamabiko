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

#include <clarus/core/loop.hpp>
using clarus::Loop;

static void run(Loop::F body, Loop &loop, bool &on) {
    body(loop);
    on = false;
}

Loop::Loop(F body, Context *parent):
    proxy(parent),
    on(true)
{
    thread.reset(
        new boost::thread(run, body, boost::ref(*this), boost::ref(this->on))
    );
}

Loop::~Loop() {
    // Nothing to do.
}

void Loop::join() {
    thread->join();
}

void Loop::stop(bool blocking) {
    if (!on) {
        return;
    }

    on = false;
    if (blocking) {
        thread->join();
    }
}

bool Loop::running() const {
    return on;
}
