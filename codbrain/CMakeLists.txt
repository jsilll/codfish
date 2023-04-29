# -- Codbrain --
add_subdirectory(src)

# -- Testing only available when building standalone --
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(tests)
    add_subdirectory(examples)
    add_subdirectory(benchmarks)
    
    # -- Docs only available when building Standalone --
    find_package(Doxygen)
    if (Doxygen_FOUND)
        add_subdirectory(docs)
    else ()
        message(STATUS "Doxygen not found, not able to build codbrain docs")
    endif ()
endif ()