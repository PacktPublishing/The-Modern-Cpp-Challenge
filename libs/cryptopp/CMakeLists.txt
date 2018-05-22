cmake_minimum_required (VERSION 3.7.0) 
project(cryptlib)

add_definitions(-DUSE_PRECOMPILED_HEADERS)
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

file(GLOB headers *.h)
file(GLOB sources *.cpp)

add_library (cryptlib STATIC ${headers} ${sources})

set_target_properties (cryptlib PROPERTIES FOLDER "00.Libs")