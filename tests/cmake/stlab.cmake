include(ExternalProject)

ExternalProject_Add(
  stlab
  PREFIX ${CMAKE_BINARY_DIR}/stlab
  GIT_REPOSITORY https://github.com/stlab/libraries.git
  GIT_TAG develop
  CMAKE_ARGS
    -Dstlab.testing=OFF
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
)

