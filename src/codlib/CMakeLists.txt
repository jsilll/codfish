# -- Source Files -- 
file(GLOB_RECURSE SOURCES "*.cpp")

# -- Codlib --
add_library(codlib STATIC ${SOURCES})

# -- Include Directory --
target_include_directories(codlib PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(codlib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(codlib PRIVATE /WX)
else ()
    target_compile_options(codlib PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(codlib PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(codlib PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(codlib PRIVATE -g -O0)
    target_compile_definitions(codlib PRIVATE DEBUG)
endif ()