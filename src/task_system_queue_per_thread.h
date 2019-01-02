#pragma once
#include <atomic>
#include <iostream>
#include <task_queue.h>
#include <thread>
#include <vector>

namespace queue_per_thread {

class task_system {
public:
    task_system() {
        for (unsigned int i = 0; i < _threads_count; ++i)
            _threads.emplace_back([&, i] { run(i); });
    }

    ~task_system() {
        for (auto &queue : _queues)
            queue.stop();
        for (auto &thread : _threads)
            thread.join();
    }

    template <typename T>
    void async(T &&task) {
        auto i = _idx++;
        _queues[i % _threads_count].push(std::forward<T>(task));
    }

private:
    void run(unsigned int i) {
        while (true) {
            task_queue::task task;
            if (!_queues[i].pop(task))
                break;
            task();
        }
    }

    const unsigned int _threads_count{std::thread::hardware_concurrency()};
    std::vector<std::thread> _threads;
    std::vector<task_queue> _queues{_threads_count};
    std::atomic<unsigned int> _idx{0};
};

} // namespace queue_per_thread
