#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

TEST_CASE("future when_all", "[future]") {
    auto argument1 = async(default_executor, [] { return 42; });
    auto argument2 = async(default_executor, [] { return string("The answer is"); });

    auto result = when_all(default_executor,
                           [](int answer, std::string text) {
                               auto result_str = text + " " + to_string(answer);
                               cout << result_str << '\n';
                               return result_str;
                           },
                           argument1, argument2);

    while (!result.get_try()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    REQUIRE(result.get_try() == "The answer is 42");
}

TEST_CASE("future when_any", "[future]") {

    auto argument1 = async(default_executor, [] { return 42; });
    auto argument2 = async(default_executor, [] { return 815; });

    auto result = when_any(default_executor,
                           [](int x, std::size_t index) {
                               auto result_str = string("The current result is ") +
                                                 to_string(x) + " " +
                                                 to_string(index);
                               cout << result_str << '\n';
                               return result_str;
                           },
                           argument1, argument2);

    // Waiting just for illustration purpose
    while (!result.get_try()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    // auto current_result =
    REQUIRE((result.get_try() == "The current result is 42 0" ||
             result.get_try() == "The current result is 815 1"));
}

