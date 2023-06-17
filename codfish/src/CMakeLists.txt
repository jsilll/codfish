# -- Source Files -- 
file(GLOB SOURCES "*.cpp")

# -- Codfish --
add_executable(codfish ${SOURCES})

# -- Include Directory --
target_include_directories(codbrain
        PUBLIC ../include)

# -- Link Libraries --
target_link_libraries(codfish
        PUBLIC codbrain)

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(codchess PRIVATE /WX)
else ()
    target_compile_options(codchess PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(codchess PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(codchess PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(codchess PRIVATE -g -O0)
    target_compile_definitions(codchess PRIVATE DEBUG)
endif ()