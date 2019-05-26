find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(element_test
  element_test.cpp
  "${SOURCES_src}"
)
target_link_libraries(element_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)


add_test(boostUnitTestsRun element_test)
