#pragma once
#include <task_queue.h>
#include <thread>
#include <vector>

namespace one_queue {

class task_system {
public:
    task_system() {
        for (unsigned int i = 0; i < _threads_count; ++i)
            _threads.emplace_back([&, i] { run(i); });
    }

    ~task_system() {
        _queue.stop();
        for (auto &thread : _threads)
            thread.join();
    }

    template <typename T>
    void async(T &&task) {
        _queue.push(std::forward<T>(task));
    }

private:
    void run(unsigned int i) {
        while (true) {
            task_queue::task task;
            if (!_queue.pop(task))
                break;
            task();
        }
    }

    const unsigned int _threads_count{std::thread::hardware_concurrency()};
    std::vector<std::thread> _threads;
    task_queue _queue;
};

} // namespace one_queue
