cmake_minimum_required(VERSION 3.0.0)
project(UXP1A-Project CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(src)
get_directory_property(
  SOURCES_src DIRECTORY src DEFINITION SOURCES
)
add_subdirectory(client)
get_directory_property(
  SOURCES_client DIRECTORY client DEFINITION SOURCES
)
add_subdirectory(tests)

add_executable(client.out
  client/client.cc
  "${SOURCES_src}"
  "${SOURCES_client}"
)
