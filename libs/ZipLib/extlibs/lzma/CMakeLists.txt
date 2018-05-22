cmake_minimum_required (VERSION 3.7.0) 
project(lzma)

if(UNIX)
   file(GLOB headers unix/*.h)
   file(GLOB sources unix/*.c)
else()
   file(GLOB headers *.h)
   file(GLOB sources *.c)
endif()

add_library (lzma STATIC ${headers} ${sources})

set_target_properties (lzma PROPERTIES FOLDER "00.Libs")