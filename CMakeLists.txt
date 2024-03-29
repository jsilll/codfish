cmake_minimum_required(VERSION 3.2...3.15)

#-------------------Setup-------------------
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(codfish VERSION 1.0 DESCRIPTION "UCI Compliant Chess Engine")

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -Wconversion -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -m64")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#-------------------Library-------------------
file(GLOB CHESS_SRC ${PROJECT_SOURCE_DIR}/src/engine/*.cpp)
file(GLOB MOVEPICKER_SRC ${PROJECT_SOURCE_DIR}/src/engine/movepicker/*.cpp)
file(GLOB MOVEGEN_SRC ${PROJECT_SOURCE_DIR}/src/engine/movegen/*.cpp)
add_library(engine ${CHESS_SRC} ${MOVEGEN_SRC} ${MOVEPICKER_SRC})
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/src)

#-------------------Executable-------------------
file(GLOB EXEC_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp
                   ${PROJECT_SOURCE_DIR}/src/interfaces/*.cpp
                   ${PROJECT_SOURCE_DIR}/src/interfaces/cli/*.cpp
                   ${PROJECT_SOURCE_DIR}/src/interfaces/uci/*.cpp
                   ${PROJECT_SOURCE_DIR}/src/interfaces/cli/commands/*.cpp
                   ${PROJECT_SOURCE_DIR}/src/interfaces/uci/commands/*.cpp
                   )
add_executable(codfish ${EXEC_SRC})
target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(codfish engine Threads::Threads )

#-------------------Tests-------------------
enable_testing()

file(GLOB TESTS_SRC ${PROJECT_SOURCE_DIR}/tests/movegen/test_*.cpp 
                    ${PROJECT_SOURCE_DIR}/tests/movepicker/test_*.cpp
                    )

foreach(TEST_SRC ${TESTS_SRC})
  get_filename_component(TEST_NAME ${TEST_SRC} NAME_WLE)
  add_executable(${TEST_NAME} ${TEST_SRC})
  target_include_directories(${TEST_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
  target_link_libraries(${TEST_NAME} engine)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
