cmake_minimum_required(VERSION 3.2...3.15)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(chess_engine VERSION 1.0 DESCRIPTION "Chess Engine")

# Engine 
file(GLOB ENGINE_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp)

add_executable(engine ${ENGINE_SRC})
target_link_libraries(engine readline)
set_target_properties(engine PROPERTIES COMPILE_FLAGS "-Ofast -Wall -Wextra")

# Tests
set(TEST_SRC ${ENGINE_SRC})
list(REMOVE_ITEM TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
list(REMOVE_ITEM TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.cpp)
list(REMOVE_ITEM TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.hpp)
list(REMOVE_ITEM TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
list(REMOVE_ITEM TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.hpp)

add_executable(test_board ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_board.cpp ${TEST_SRC})
add_executable(test_movegen ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_movegen.cpp ${TEST_SRC})

enable_testing()
add_test(NAME test_board COMMAND test_board)
add_test(NAME test_movegen COMMAND test_movegen)