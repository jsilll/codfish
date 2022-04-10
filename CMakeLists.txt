cmake_minimum_required(VERSION 3.2...3.15)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(chess_engine VERSION 1.0 DESCRIPTION "Chess Engine")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Engine Library 
file(GLOB ENGINE_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp)
set(ENGINE_SRC ${ENGINE_SRC})
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.hpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.hpp)
add_library(engine ${ENGINE_SRC})

# Engine Executable
add_executable(engine_exec ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
target_link_libraries(engine_exec readline)
target_link_libraries(engine_exec engine)

# Tests
add_executable(tables_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/tables_tests.cpp)
target_link_libraries(tables_tests engine)
add_executable(board_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/board_tests.cpp)
target_link_libraries(board_tests engine)
add_executable(perft_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/perft_tests.cpp)
target_link_libraries(perft_tests engine)

enable_testing()
add_test(NAME tables_tests COMMAND tables_tests)
add_test(NAME board_tests COMMAND board_tests)
add_test(NAME perft_tests COMMAND perft_tests)