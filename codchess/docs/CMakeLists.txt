# -- Examples --
add_subdirectory(examples)

# -- Docs only available when building Standalone --
find_package(Doxygen)
if (Doxygen_FOUND)
    # -- Setting up Doxygen --
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # -- Public Headers --
    file(GLOB_RECURSE HEADERS ../include/codchess/*.hpp)

    # -- Building Documentation --
    doxygen_add_docs(docs_codchess ${HEADERS} COMMENT "Generating Documentation for CodChess")
else ()
    message(STATUS "Doxygen not found, not able to build codchess docs")
endif ()