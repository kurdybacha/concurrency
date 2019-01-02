include(ExternalProject)

ExternalProject_Add(
  picosha2
  PREFIX ${CMAKE_BINARY_DIR}/picosha2
  URL "https://raw.githubusercontent.com/okdshin/PicoSHA2/master/picosha2.h"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
  DOWNLOAD_NO_EXTRACT ON
)

# Expose variable PICOSHA2_INCLUDE_DIR to parent scope
ExternalProject_Get_Property(picosha2 DOWNLOAD_DIR)
set(PICOSHA2_INCLUDE_DIR ${DOWNLOAD_DIR} CACHE INTERNAL "Path to include picosha2")
