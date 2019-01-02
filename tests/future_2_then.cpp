#include <catch.hpp>
#include <iostream>
#include <thread>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

TEST_CASE("future then", "[future]") {

    auto x = async(default_executor, [] { return 42; });

    auto y = x.then([](int x) {
        printf("Result %d \n", x);
        return x * 2;
    });

    while (!y.get_try()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    REQUIRE(x.get_try() == 42);
    REQUIRE(y.get_try() == 42 * 2);
}





















TEST_CASE("future then exception", "[future]") {

    //  auto x = async(default_executor, [] {
    //   throw runtime_error("Vogons did arrive!");
    //   return 42;
    // });
    //
    // auto r = x.recover([](future<int> f) {
    //     try {
    //       auto answer = *f.get_try();
    //       cout << "The answer is " << answer << '\n';
    //     }
    //     catch (const exception& ex) {
    //       cout << "The error \"" << ex.what() << "\" happened!\n";
    //     }
    //   });
    //
    // // Waiting just for illustration purpose
    // while (!r.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    auto x = async(default_executor, [] { return 42; });

    auto y = x.then([](int x) {
        throw runtime_error(":(");
        return x;
    }).recover([](auto result) {
        try {
            return *result.get_try();
        } catch (const exception &e) {
            cout << "The error \"" << e.what() << "\" happened\n";
        }
        return 200;
    });
    while (!y.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
    cout << "The error \"" << *y.get_try() << "\" happened\n";

    // int result{0};
    // auto f1 =
    //     async(default_executor, []() -> int { throw runtime_error("failure"); });
    // auto f2 = async(default_executor, [] { return 42; });
    //
    // auto sut = when_all(default_executor, [](int x, int y) { return x + y; }, f1, f2)
    //           .recover([](auto error) {
    //               if (error.error())
    //                   return 815;
    //               else
    //                   return 0;
    //           })
    //           .then([&](int x) { result = x; });
    // // Waiting just for illustration purpose
    // while (!sut.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
    // REQUIRE(result == 815);
}
