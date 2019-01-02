#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

TEST_CASE("future split", "[future]") {

    auto x = async(default_executor, [] { return 42; });

    auto c1 = x.then([](int x) {
        printf("Split A %d \n", x);
        return x * 2;
    });
    auto c2 = x.then([](int x) {
        printf("Split B %d \n", x);
        return x * 3;
    });

    while (!c1.get_try()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    while (!c2.get_try()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    REQUIRE(c1.get_try() == 42 * 2);
    REQUIRE(c2.get_try() == 42 * 3);
}

