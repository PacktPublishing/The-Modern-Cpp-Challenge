cmake_minimum_required (VERSION 3.7.0) 
project(bzip2)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library (bzip2 STATIC ${headers} ${sources})

set_target_properties (bzip2 PROPERTIES FOLDER "00.Libs")