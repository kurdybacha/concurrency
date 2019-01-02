set(target unit_tests)
project(${target} C CXX)

include(cmake/catch2.cmake)
include(cmake/picosha2.cmake)
# include(cmake/benchmark.cmake)
include(cmake/stlab.cmake)

# find_package(benchmark)

# Sources
set(headers
)

set(sources
    task_system.cpp
    future_1_simple.cpp
    future_2_then.cpp
    future_3_join.cpp
    future_4_split.cpp
    channel_1_simple.cpp
    channel_2_stateful.cpp
    channel_3_join.cpp
)

# Build executable
add_executable(${target}
    ${sources}
    ${headers}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})

add_dependencies(${target} stlab catch2 picosha2)

#
# Libraries
#
target_include_directories(${target}
    PRIVATE
    ${CMAKE_BINARY_DIR}/include
    ${CATCH2_INCLUDE_DIR}
    ${PICOSHA2_INCLUDE_DIR}
    # ${BENCHMARK_INCLUDES}
)
target_link_libraries(${target}
    PRIVATE
    ${META_PROJECT_NAME}::concurrency
    ${BENCHMARK_LIBRARIES}
    pthread
)
