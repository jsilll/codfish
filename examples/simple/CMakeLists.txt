# -- Sources --
file(GLOB_RECURSE SOURCES *.cpp)

# -- Executable --
add_executable(minimal ${SOURCES})

target_link_libraries(minimal PRIVATE codchess)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(minimal PRIVATE /WX)
else ()
    target_compile_options(minimal PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(minimal PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(minimal PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(minimal PRIVATE -g -O0)
    target_compile_definitions(minimal PRIVATE DEBUG)
endif ()