#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <utility>

class task_queue {
public:
    using task = std::function<void()>;

    void stop() {
        {
            std::unique_lock<std::mutex> lock{_mutex};
            _stop = true;
        }
        _work_ready.notify_all();
    }

    bool try_pop(task &t) {
        std::unique_lock<std::mutex> lock{_mutex, std::try_to_lock};
        if (!lock || _queue.empty())
            return false;
        t = std::move(_queue.front());
        _queue.pop_front();
        return true;
    }

    template <typename T>
    bool try_push(T &&t) {
        {
            std::unique_lock<std::mutex> lock{_mutex, std::try_to_lock};
            if (!lock)
                return false;
            _queue.emplace_back(std::forward<T>(t));
        }
        _work_ready.notify_one();
        return true;
    }

    bool pop(task &t) {
        std::unique_lock<std::mutex> lock{_mutex};
        _work_ready.wait(lock, [this] {
            return !_queue.empty() || _stop;
        });
        if (_queue.empty())
            return false;
        t = std::move(_queue.front());
        _queue.pop_front();
        return true;
    }

    template <typename T>
    void push(T &&t) {
        {
            std::lock_guard<std::mutex> lock{_mutex};
            _queue.emplace_back(std::forward<T>(t));
        }
        _work_ready.notify_one();
    }

private:
    std::deque<task> _queue;
    std::mutex _mutex;
    std::condition_variable _work_ready;
    bool _stop{false};
};
