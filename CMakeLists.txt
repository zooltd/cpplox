cmake_minimum_required(VERSION 3.17)
project(cpplox VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(src)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}.lib)

find_program(GCOV_PATH gcov)
if(NOT GCOV_PATH)
  message(FATAL_ERROR "Code coverage analysis requires gcov!")
endif()

enable_testing()

add_subdirectory(test)
