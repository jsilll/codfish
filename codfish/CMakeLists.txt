# -- Codfish Library --
add_subdirectory(src)

# -- Codfish Executable --
add_subdirectory(app)

# -- Testing only available when building standalone --
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(tests)
    
    # -- Docs only available when building Standalone --
    find_package(Doxygen)
    if (Doxygen_FOUND)
        add_subdirectory(docs)
    else ()
        message(STATUS "Doxygen not found, not able to build codfish docs")
    endif ()
endif ()
