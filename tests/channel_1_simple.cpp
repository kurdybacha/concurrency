#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace std;
using namespace stlab;

TEST_CASE("channel simple", "[channel]") {

    auto [send, receive] = channel<int>(default_executor);

    std::atomic_bool done{false};

    auto printer = [&_done = done](int x) { // attaching process to the receiving end of channel
        cout << x << '\n';
        if (x == 3)
            _done = true;
    };

    auto printer_process = receive | std::ref(printer);

    // It is necessary to mark the receiver side as ready, when all connections are
    // established
    receive.set_ready(); // no more processes will be attached, process starts work

    send(1); // sending into channel
    send(2);
    send(3);

    while (!done.load()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    REQUIRE(done.load());
}
