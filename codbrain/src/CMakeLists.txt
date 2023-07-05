# -- Source Files -- 
file(GLOB_RECURSE SOURCES "*.cpp")

# -- Find Boost --
find_package(Boost REQUIRED)

# -- Codchess --
add_library(codbrain STATIC ${SOURCES})

# -- Include Directory --
target_include_directories(codbrain
        PUBLIC ../include)

# -- Link Libraries --
target_link_libraries(codbrain
        PUBLIC codchess)

target_link_libraries(codbrain
        PUBLIC ${Boost_LIBRARIES})

# -- All Warnings as Errors --
if (MSVC)
    target_compile_options(codbrain PRIVATE /WX)
else ()
    target_compile_options(codbrain PRIVATE -Werror)
endif ()

# -- Release and Debug Flags --
if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(codbrain PRIVATE -O3 -march=native)
elseif (CMAKE_BUILD_TYPE MATCHES ReleaseCrossPlatform)
    target_compile_options(codbrain PRIVATE -O3 -march=x86-64)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(codbrain PRIVATE -g -O0)
    target_compile_definitions(codbrain PRIVATE DEBUG)
endif ()