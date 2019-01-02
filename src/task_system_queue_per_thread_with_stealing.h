#pragma once
#include <atomic>
#include <task_queue.h>
#include <thread>
#include <vector>

namespace queue_per_thread_with_stealing {

class task_system {
public:
    task_system() {
        for (unsigned int i = 0; i != _threads_count; ++i)
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
        for (unsigned int t = 0; t != _threads_count; ++t) {
            if (_queues[(i + t) % _threads_count].try_push(std::forward<T>(task)))
                return;
        }
        _queues[i % _threads_count].push(std::forward<T>(task));
    }

private:
    void run(unsigned int i) {
        while (true) {
            task_queue::task task;
            for (unsigned int t = 0; t != _threads_count * 16; ++t) {
                if (_queues[(i + t) % _threads_count].try_pop(task))
                    break;
            }
            if (!task && !_queues[i].pop(task))
                break;
            task();
        }
    }

    const unsigned int _threads_count{std::thread::hardware_concurrency()};
    std::vector<std::thread> _threads;
    std::vector<task_queue> _queues{_threads_count};
    std::atomic<unsigned int> _idx{0};
};

} // namespace queue_per_thread_with_stealing
