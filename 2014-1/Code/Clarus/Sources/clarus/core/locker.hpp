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

#ifndef CLARUS_CORE_LOCKER_HPP
#define CLARUS_CORE_LOCKER_HPP

#include <boost/thread.hpp>

namespace clarus {
    template<typename T> class locked_ptr;

    class Locker;
}

template<typename T> class clarus::locked_ptr: public boost::shared_ptr<T> {
    /*
    Used to ensure the encapsulated object won't be deallocated if it was already under
    shared pointer management.
    */
    boost::shared_ptr<T> value;

public:
    template<typename F, typename G> locked_ptr(T &value, F lock, G unlock):
        boost::shared_ptr<T>(&value, unlock)
    {
        lock();
    };

    template<typename F, typename G> locked_ptr(boost::shared_ptr<T> pointer, F lock, G unlock):
        boost::shared_ptr<T>(pointer.get(), unlock),
        value(pointer)
    {
        lock();
    };
};

class clarus::Locker {
    boost::shared_mutex locker;

    boost::condition_variable notification;

    boost::mutex waiter;

    int tally;

public:
    Locker();

    void lock();

    void unlock();

    void wait();

    void notify();

    template<typename T> locked_ptr<T> read(T &value) {
        return locked_ptr<T>(
            value,
            boost::bind(&boost::shared_mutex::lock_shared, &locker),
            boost::bind(&boost::shared_mutex::unlock_shared, &locker)
        );
    }

    template<typename T> locked_ptr<T> read(boost::shared_ptr<T> value) {
        return locked_ptr<T>(
            value,
            boost::bind(&boost::shared_mutex::lock_shared, &locker),
            boost::bind(&boost::shared_mutex::unlock_shared, &locker)
        );
    }

    template<typename T> locked_ptr<T> write(T &value) {
        return locked_ptr<T>(
            value,
            boost::bind(&boost::shared_mutex::lock, &locker),
            boost::bind(&boost::shared_mutex::unlock, &locker)
        );
    }

    template<typename T> locked_ptr<T> write(boost::shared_ptr<T> value) {
        return locked_ptr<T>(
            value,
            boost::bind(&boost::shared_mutex::lock, &locker),
            boost::bind(&boost::shared_mutex::unlock, &locker)
        );
    }
};

#endif
