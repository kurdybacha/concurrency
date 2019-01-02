include(ExternalProject)

ExternalProject_Add(
  catch2
  PREFIX ${CMAKE_BINARY_DIR}/catch2
  URL "https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
  DOWNLOAD_NO_EXTRACT ON
)

# Expose variable CATCH_INCLUDE_DIR to parent scope
ExternalProject_Get_Property(catch2 DOWNLOAD_DIR)
set(CATCH2_INCLUDE_DIR ${DOWNLOAD_DIR} CACHE INTERNAL "Path to include catch2")
