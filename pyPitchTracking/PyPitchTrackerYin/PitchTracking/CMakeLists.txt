cmake_minimum_required(VERSION 2.6)
project(PitchTracking CXX)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10)

set(test_path "test")
set(test_files
    ${test_path}/main.cpp
)
set(src_path "src")
set(src_files
    ${src_path}/PitchTracker.cpp
    ${src_path}/utils.cpp
)
set(include_path "include")
set(include_files
    ${include_path}/PitchTracker.h
    ${include_path}/utils.h
)

# Dependencies

set(ar_libgtest /usr/local/lib/libgtest.a)

# Frameworks

set(include_options
    -I${PROJECT_SOURCE_DIR}/${include_path}/
    -I/usr/local/include)

#project compile options
set(project_compile_options
    ${include_options} -std=gnu++11 -stdlib=libc++)

# library target
# **************

# including include_files is not needed, because cmake automatically generate dependencies
# this is only needed for generating Xcode projects including header files
add_library(PitchTracking ${src_files} ${include_files})

target_compile_options(PitchTracking PUBLIC ${project_compile_options}) 

# test
# ****************
add_executable(PitchTracking_test ${test_files})

target_compile_options(PitchTracking_test PUBLIC ${project_compile_options} )
target_link_libraries(PitchTracking_test 
    ${ar_libgtest}
    PitchTracking
)

