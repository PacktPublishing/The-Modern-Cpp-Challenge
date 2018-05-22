cmake_minimum_required (VERSION 3.7.0) 
project(zlib)

file(GLOB headers *.h)
file(GLOB sources *.c)

add_library (zlib STATIC ${headers} ${sources})

set_target_properties (zlib PROPERTIES FOLDER "00.Libs")