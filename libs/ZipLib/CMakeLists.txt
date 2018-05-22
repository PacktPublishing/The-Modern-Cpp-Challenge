cmake_minimum_required (VERSION 3.7.0) 
project(ZipLib)

ADD_SUBDIRECTORY(extlibs/bzip2)
ADD_SUBDIRECTORY(extlibs/lzma)
ADD_SUBDIRECTORY(extlibs/zlib)

file(GLOB headers *.h detail/*.h)
file(GLOB sources *.cpp detail/*.cpp)

add_library (ZipLib STATIC ${headers} ${sources})

target_link_libraries(ZipLib bzip2)
target_link_libraries(ZipLib lzma)
target_link_libraries(ZipLib zlib)

set_target_properties (ZipLib PROPERTIES FOLDER "00.Libs")