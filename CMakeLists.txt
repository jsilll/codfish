cmake_minimum_required(VERSION 3.2...3.15)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(chess_engine VERSION 1.0 DESCRIPTION "Chess Engine")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Adding Engine Library
file(GLOB ENGINE_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp)
set(ENGINE_SRC ${ENGINE_SRC})
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.hpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.hpp)
add_library(engine ${ENGINE_SRC})

# Adding Engine Executable
add_executable(engine_exec ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/uci.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
target_link_libraries(engine_exec engine)
target_link_libraries(engine_exec readline)

# Adding Tests
add_executable(test_board ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_board.cpp)
target_link_libraries(test_board engine)

add_executable(test_perft ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_perft.cpp)
target_link_libraries(test_perft engine)

add_executable(test_tables ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_tables.cpp)
target_link_libraries(test_tables engine)

enable_testing()
add_test(NAME test_board COMMAND test_board)
add_test(NAME test_perft COMMAND test_perft)
add_test(NAME test_tables COMMAND test_tables)