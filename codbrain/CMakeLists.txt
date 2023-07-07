# -- Codbrain --
add_subdirectory(src)

# -- Testing only available when building standalone --
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(test)
    add_subdirectory(docs)
    add_subdirectory(benchmark)
endif ()