#include <clarus/core/tasks.hpp>

Looper &Tasks::peek() {
    locked_ptr<Loopers> loopers = locker.read(this->loopers);
    return loopers->front();
}

void Tasks::pop() {
    locked_ptr<Loopers> loopers = locker.write(this->loopers);
    loopers->pop_front();
}

size_t Tasks::size() {
    locked_ptr<Loopers> loopers = locker.read(this->loopers);
    return loopers->size();
}

void Tasks::start(Task task) {
    locked_ptr<Loopers> loopers = locker.write(this->loopers);
    loopers->push_back(Looper());
    Looper &looper = loopers->back();
    looper.start(boost::bind(task, boost::ref(*this), _1));
}

void Tasks::stop() {
    locked_ptr<Loopers> loopers = locker.read(this->loopers);
    for (Loopers::iterator i = loopers->begin(), n = loopers->end(); i != n; ++i) {
        Looper &looper = (*i);
        looper.stop(false);
    }
}

void Tasks::join() {
    while (size() > 0) {
        Looper &looper = peek();
        looper.join();
        pop();
    }
}
