add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the ${PROJECT_NAME}.test target
add_executable(${PROJECT_NAME}.test RunnerTest.cpp)

# linking ${PROJECT_NAME}.test with ${PROJECT_NAME} which will be tested
target_link_libraries(${PROJECT_NAME}.test PRIVATE ${PROJECT_NAME}.lib)

target_link_libraries(${PROJECT_NAME}.test PRIVATE gtest_main)
