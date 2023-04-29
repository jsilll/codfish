# -- Benchmark Sources --
file(GLOB SOURCES bench_*.cpp)

# -- Benchmark Executables --
foreach (SOURCE ${SOURCES})
    get_filename_component(BENCH ${SOURCE} NAME_WE)

    add_executable(${BENCH} ${SOURCE})

    target_link_libraries(${BENCH}
            PRIVATE Catch2::Catch2WithMain
            PRIVATE codchess)

    add_test(${BENCH} ${BENCH})
endforeach ()