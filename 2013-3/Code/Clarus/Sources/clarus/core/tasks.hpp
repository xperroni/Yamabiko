#ifndef CLARUS_CORE_TASKS_HPP
#define CLARUS_CORE_TASKS_HPP

#include <clarus/core/locker.hpp>
#include <clarus/core/looper.hpp>
#include <clarus/core/table.hpp>

#include <boost/function.hpp>

#include <deque>
#include <string>

/* Forward declaration, needed for the function type declaration below. */
class Tasks;

/* An individual task. */
typedef boost::function<void(Tasks&, bool &running)> Task;

class Tasks {
    typedef std::deque<Looper> Loopers;

    Table table;

    Loopers loopers;

    Looper &peek();

    void pop();

    size_t size();

public:
    // Locker object, can be used for synchronizing across tasks.
    Locker locker;

    /*
    Runs a new task in a new background Looper.
    */
    void start(Task task);

    /*
    Stop all running tasks. Non-blocking operation.
    */
    void stop();

    /*
    Continously join() the running Loopers one after the other, until there
    are none left.
    */
    void join();

    template<typename T> T &get(const std::string &name);

    template<typename T> T &set(const std::string &name, const T &value);

    template<typename T> T &set(const std::string &name, T *value);
};

template<typename T> T &Tasks::get(const std::string &name) {
    return locker.read(table)->get<T>(name);
}

template<typename T> T &Tasks::set(const std::string &name, const T &value) {
    return locker.write(table)->set<T>(name, value);
}

template<typename T> T &Tasks::set(const std::string &name, T *value) {
    return locker.write(table)->set<T>(name, value);
}

#endif
