cmake_minimum_required(VERSION 3.2...3.15)

project(chess_engine
        VERSION 1.0
        DESCRIPTION "Chess Engine"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

file(GLOB SRC
       ${PROJECT_SOURCE_DIR}/src/*.cpp
       ${PROJECT_SOURCE_DIR}/src/*.hpp
)

add_executable(main ${SRC})
target_link_libraries(main readline)