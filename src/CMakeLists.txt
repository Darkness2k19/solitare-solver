# SOURCE DIRS
add_subdirectory(solver)
add_subdirectory(utility)

# MAIN

get_property(prj_sources GLOBAL PROPERTY PROJECT_SOURCES)
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${prj_sources})
