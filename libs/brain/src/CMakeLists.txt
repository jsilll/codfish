# -- Source Files -- 
file(GLOB_RECURSE SOURCES "*.cpp")

# -- Codchess --
add_library(brain SHARED ${SOURCES})

# -- Include Directory --
target_include_directories(brain
        PUBLIC ../include)

# -- Link Libraries --
target_link_libraries(brain
        PUBLIC core
        PRIVATE uci)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(brain PRIVATE /WX)
else ()
    target_compile_options(brain PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(brain PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(brain PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(brain PRIVATE -g -O0)
    target_compile_definitions(brain PRIVATE DEBUG)
endif ()