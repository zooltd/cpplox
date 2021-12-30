add_subdirectory(lib/magic_enum)
include_directories(lib/magic_enum/include)

add_library(${PROJECT_NAME}.lib)

target_sources(${PROJECT_NAME}.lib
  PRIVATE
    Logger.cpp
    Runner.cpp
    Scanner.cpp
    Token.cpp
    
  PUBLIC
    Logger.h
    Runner.h
    Scanner.h
    Token.h
   )

target_link_libraries(${PROJECT_NAME}.lib PRIVATE magic_enum)

target_include_directories(${PROJECT_NAME}.lib
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

# we use this to get code coverage
# flags are only valid with the GNU compiler and on Linux
if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(${PROJECT_NAME}.lib
    PUBLIC
      "--coverage"
    )
  target_link_options(${PROJECT_NAME}.lib
    INTERFACE
      "--coverage"
    )
endif()