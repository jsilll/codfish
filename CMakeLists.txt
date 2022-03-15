cmake_minimum_required(VERSION 3.2...3.15)

project(chess_engine
        VERSION 1.0
        DESCRIPTION "Chess Engine"
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_FLAGS "-Ofast -Wall -Wextra")

# Engine Executable
file(GLOB ENGINE_SRC
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.hpp
        )
add_executable(engine ${ENGINE_SRC})
target_link_libraries(engine readline)

# Engine Library
set(ENGINE_LIB_SRC ${ENGINE_SRC})
list(REMOVE_ITEM ENGINE_LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_library(engine-lib ${ENGINE_LIB_SRC})

# Tests Executable
file(GLOB TESTS_SRC
        ${PROJECT_SOURCE_DIR}/tests/*.cpp
        ${PROJECT_SOURCE_DIR}/tests/*.hpp
        )
add_executable(tests ${TESTS_SRC})
target_link_libraries(tests engine-lib)