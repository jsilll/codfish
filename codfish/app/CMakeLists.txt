# -- Source Files -- 
file(GLOB SOURCES "*.cpp")

# -- Codfish Engine --
add_executable(engine ${SOURCES})

# -- Link Libraries --
target_link_libraries(engine
        PUBLIC codfish)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(engine PRIVATE /WX)
else ()
    target_compile_options(engine PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(engine PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(engine PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(engine PRIVATE -g -O0)
    target_compile_definitions(engine PRIVATE DEBUG)
endif ()