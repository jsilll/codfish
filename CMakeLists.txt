cmake_minimum_required(VERSION 3.21)

# -- FetchContent --
Include(FetchContent)

# -- Setup --
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT ON)

# -- Project Metadata --
project(Codfish
        VERSION 0.1.0
        DESCRIPTION "Codfish"
        HOMEPAGE_URL "https://github.com/jsilll/codfish"
        LANGUAGES CXX)

# -- Baseline Flags --
if (MSVC)
    add_compile_options(/W4 /w14640)
else ()
    add_compile_options(
            -Wall -Wextra -Wshadow -Wunused -Wnon-virtual-dtor -pedantic)
endif ()

# -- When Building Standalone --
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # -- Testing Only Available When Building Standalone --
    include(CTest)
    enable_testing()

    # -- Fetch Catch2 --
    FetchContent_Declare(
            Catch2
            GIT_TAG v3.3.2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    )
    FetchContent_MakeAvailable(Catch2)

    # -- Ensure that the -std=c++xx flag is used --
    set(CMAKE_CXX_EXTENSIONS OFF)

    # -- Support for folders in IDEs --
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif ()

# -- Uci Library --
add_subdirectory(uci)

# -- Chess Library --
add_subdirectory(chess)

# -- Brain Library -- 
add_subdirectory(brain)

# -- Applications --
add_subdirectory(apps)
