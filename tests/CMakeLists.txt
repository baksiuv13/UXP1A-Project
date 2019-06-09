
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(element_test.out
  element_test.cc
  "${SOURCES_src}"
)
target_link_libraries(element_test.out
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_executable(tuple_test.out
  tuple_test.cc
  "${SOURCES_src}"
)
target_link_libraries(tuple_test.out
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_executable(semaphore_test.out
  semaphore-test.cc
  "${SOURCES_src}"
)
target_link_libraries(semaphore_test.out
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)


add_executable(use_linda.out
  use_linda.cc
  "${SOURCES_src}"
)

add_test(Element element_test.out)
add_test(Tuple tuple_test.out)
add_test(Semaphore semaphore_test.out)
add_test(Example_use_linda use_linda.out)

