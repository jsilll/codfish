# -- Test Sources --
file(GLOB SOURCES *.cpp)

# -- Test Executables --
foreach (SOURCE ${SOURCES})
    get_filename_component(TEST ${SOURCE} NAME_WE)

    add_executable(${TEST} ${SOURCE})

    target_link_libraries(${TEST}
            PRIVATE Catch2::Catch2WithMain
            PRIVATE codlib)

    add_test(${TEST} ${TEST})
endforeach ()
