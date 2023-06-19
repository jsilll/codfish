# -- Source Files -- 
file(GLOB_RECURSE SOURCES "*.cpp")

# -- Codchess --
add_library(codfish STATIC ${SOURCES})

# -- Include Directory --
target_include_directories(codfish
        PUBLIC ../include)

# -- Link Libraries --
target_link_libraries(codfish
        PUBLIC codbrain)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(codfish PRIVATE /WX)
else ()
    target_compile_options(codfish PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(codfish PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(codfish PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(codfish PRIVATE -g -O0)
    target_compile_definitions(codfish PRIVATE DEBUG)
endif ()