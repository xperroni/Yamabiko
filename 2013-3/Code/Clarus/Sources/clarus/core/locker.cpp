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

#include <clarus/core/locker.hpp>

Locker::Locker():
    tally(0)
{
    // Nothing to do
}

void Locker::lock() {
    locker.lock();
}

void Locker::unlock() {
    locker.unlock();
}

void Locker::wait() {
    /*
    The tally is a number that gets incremented every time notify() is called. The
    variable notch stores the value of tally by the time the wait started. After
    notify() is called, tally and notch will have different values, and the wait
    will end.

    This scheme allows several Lockers to each call wait() on the same Locker
    object, and be assured that they will be released from the wait after notify()
    is called, even if wait() is called agan before the Locker checks the
    notification state.
    */

    boost::unique_lock<boost::mutex> lock(waiter);
    for (int notch = tally; tally == notch;) {
        notification.wait(lock);
    }
}

void Locker::notify() {
    /* Change the tally inside a synchronized scope */ {
        boost::lock_guard<boost::mutex> lock(waiter);
        tally++;
    }

    notification.notify_all();
}
