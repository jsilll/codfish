# -- Sources --
file(GLOB_RECURSE SOURCES *.cpp)

# -- Executable --
add_executable(minimal ${SOURCES})

target_link_libraries(minimal PRIVATE codchess)