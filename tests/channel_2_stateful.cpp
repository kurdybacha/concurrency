#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace std;
using namespace stlab;

struct adder {
    int _sum = 0;
    process_state_scheduled _state = await_forever;

    void await(int x) {
        _sum += x;
        if (x == 0) {
            _state = yield_immediate;
        }
    }

    int yield() {
        int result = _sum;
        _sum = 0;
        _state = await_forever;
        return result;
    }

    auto state() const { return _state; }
};

TEST_CASE("channel stateful", "[channel]") {

    auto [send, receiver] = channel<int>(default_executor);

    std::atomic_bool done{false};

    auto calculator = receiver | adder{} | [&_done = done](int x) {
        std::cout << x << '\n';
        _done = true;
    };

    receiver.set_ready();

    send(1);
    send(2);
    send(3);
    send(0);

    // Waiting just for illustrational purpose
    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
