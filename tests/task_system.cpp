#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <picosha2.h>
#include <random>
#include <task_system_one_queue.h>
#include <task_system_queue_per_thread.h>
#include <task_system_queue_per_thread_with_stealing.h>

namespace {

inline bool odd(int n) { return n & 0x01; }

inline int half(int n) { return n >>= 1; }

// Ancient Egyptian multiplication algorithm,
// also known as Russian peasant multiplication
int multiply(int n, int a) {
    int result = 0;
    if (n == 0 || a == 0)
        return 0;
    while (true) {
        if (odd(n)) {
            result += a;
            if (n == 1)
                return result;
        }
        n = half(n);
        a += a;
    }
}

} // namespace

TEST_CASE("let's test multiply a bit", "[multiply]") {
    auto[multiplicand, multiplier] = GENERATE(table<int, int>(
        {{0, 100}, {100, 0}, {1, 56}, {56, 1}, {6, 12}, {4294967296, 2}}));
    REQUIRE(multiply(multiplicand, multiplier) == multiplicand * multiplier);
}

template <typename T>
struct TaskRunner : public T {
    using T::async;
};

template <typename T>
struct MultiplyTasks : public TaskRunner<T> {

    struct multiply_task {
        int n;
        int a;
        void operator()() { multiply(n, a); }
    };

    void operator()() {
        for (int n = 0; n < 4000; ++n) {
            for (int a = 0; a < 4000; ++a) {
                TaskRunner<T>::async(multiply_task{n, a});
            }
        }
    }
};

template <typename T>
struct HashSHA2Tasks : public TaskRunner<T> {
    struct sha2_hash_task {
        std::string src;
        std::string operator()() {
            std::vector<unsigned char> hash(picosha2::k_digest_size);
            picosha2::hash256(src.begin(), src.end(), hash.begin(), hash.end());
            return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
        }
    };

    void operator()() {
        for (int i = 0; i < 2000000; ++i)
            TaskRunner<T>::async(sha2_hash_task{"Random string"});
    }
};

template <typename T>
struct Benchmark {
    void operator()() {
        auto start = std::chrono::high_resolution_clock::now();
        auto task_creation = std::chrono::milliseconds{0};
        BENCHMARK("Benchmark Runner") {
            T task_runner;
            // don't count task_system construction in the result.
            start = std::chrono::high_resolution_clock::now();
            task_runner();
            task_creation = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start);
        }
        auto total = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start);
        std::clog << "Tasks created in : " << task_creation.count() << std::endl;
        std::clog << "Total run in : " << total.count() << std::endl;
    }
};

TEST_CASE("multiply - one queue", "[multiply]") {
    Benchmark<MultiplyTasks<one_queue::task_system>>{}();
}

TEST_CASE("multiply - queue per thread", "[multiply]") {
    Benchmark<MultiplyTasks<queue_per_thread::task_system>>{}();
}

TEST_CASE("multiply - queue per thread with stealing", "[multiply]") {
    Benchmark<MultiplyTasks<queue_per_thread_with_stealing::task_system>>{}();
}

TEST_CASE("sha2 - one queue", "[sha2]") {
    Benchmark<HashSHA2Tasks<one_queue::task_system>>{}();
}

TEST_CASE("sha2 - queue per thread", "[sha2]") {
    Benchmark<HashSHA2Tasks<queue_per_thread::task_system>>{}();
}

TEST_CASE("sha2 - queue per thread with stealing", "[sha2]") {
    Benchmark<HashSHA2Tasks<queue_per_thread_with_stealing::task_system>>{}();
}
