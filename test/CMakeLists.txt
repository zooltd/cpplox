add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the ${PROJECT_NAME}.test target
add_executable(${PROJECT_NAME}.test InterpreterTest.cpp)

# linking ${PROJECT_NAME}.test with ${PROJECT_NAME} which will be tested
target_link_libraries(${PROJECT_NAME}.test PRIVATE ${PROJECT_NAME}.lib)

target_link_libraries(${PROJECT_NAME}.test PRIVATE gtest_main)

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}.test
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )