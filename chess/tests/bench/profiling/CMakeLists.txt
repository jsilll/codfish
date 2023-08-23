# -- Profile Sources --
file(GLOB SOURCES profile_*.cpp)

# -- Profile Executables --
foreach (SOURCE ${SOURCES})
    get_filename_component(PROFILE ${SOURCE} NAME_WE)

    add_executable(${PROFILE} ${SOURCE})

    target_link_libraries(${PROFILE}
            PRIVATE codchess)
endforeach ()