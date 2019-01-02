#include <catch.hpp>
#include <thread>
#include <chrono>
#include <future>

TEST_CASE("future simple", "[future]") {

    auto future = std::async(std::launch::async, []() { return 42; });

    // Do something else e.g. get same rest
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // And check the result "in the future"
    auto answer_to_everything = future.get();

    REQUIRE(answer_to_everything == 42);
}

