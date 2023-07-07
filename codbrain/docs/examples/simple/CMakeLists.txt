# -- Sources --
file(GLOB_RECURSE SOURCES *.cpp)

# -- Executable --
add_executable(example_simple_codbrain ${SOURCES})

target_link_libraries(example_simple_codbrain PRIVATE codbrain)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(example_simple_codbrain PRIVATE /WX)
else ()
    target_compile_options(example_simple_codbrain PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(example_simple_codbrain PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(example_simple_codbrain PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(example_simple_codbrain PRIVATE -g -O0)
    target_compile_definitions(example_simple_codbrain PRIVATE DEBUG)
endif ()