# -- Sources --
file(GLOB_RECURSE SOURCES *.cpp)

# -- Executable --
add_executable(codbrain_example_simple ${SOURCES})

target_link_libraries(codbrain_example_simple PRIVATE codbrain)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(codbrain_example_simple PRIVATE /WX)
else ()
    target_compile_options(codbrain_example_simple PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(codbrain_example_simple PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(codbrain_example_simple PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(codbrain_example_simple PRIVATE -g -O0)
    target_compile_definitions(codbrain_example_simple PRIVATE DEBUG)
endif ()