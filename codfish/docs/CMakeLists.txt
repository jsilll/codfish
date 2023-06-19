# -- Setting up Doxygen --
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_LATEX NO)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# -- Source Code --
file(GLOB_RECURSE HEADERS ../src/*.hpp)
file(GLOB_RECURSE SOURCES ../src/*.cpp)
file(GLOB_RECURSE TESTS ../include/codfish/*.hpp)

# -- Building Documentation --
doxygen_add_docs(codfish-docs ${HEADERS} COMMENT "Generating Documentation for Codfish")