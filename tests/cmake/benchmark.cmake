include(ExternalProject)

ExternalProject_Add(
  benchmark
  PREFIX ${CMAKE_BINARY_DIR}/benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
    -DCMAKE_BUILD_TYPE=Release
    -DBENCHMARK_ENABLE_GTEST_TESTS=OFF
)

set(BENCHMARK_LIBRARIES "${CMAKE_BINARY_DIR}/lib/libbenchmark.a")
set(BENCHMARK_INCLUDES "${CMAKE_BINARY_DIR}/include/benchmark")
