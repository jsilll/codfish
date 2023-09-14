# -- Source Files -- 
file(GLOB_RECURSE SOURCES "*.cpp")

# -- Uci --
add_library(uci SHARED ${SOURCES})

# -- Include Directory --
target_include_directories(uci
        PUBLIC ../include)

# -- Link Libraries --
target_link_libraries(uci
        PUBLIC ${Boost_LIBRARIES})

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(uci PRIVATE /WX)
else ()
    target_compile_options(uci PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(uci PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(uci PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(uci PRIVATE -g -O0)
    target_compile_definitions(uci PRIVATE DEBUG)
endif ()