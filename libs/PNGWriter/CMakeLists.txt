cmake_minimum_required (VERSION 3.7.0) 
project(PngWriter)

include_directories(${LIBS_PATH}/PDF-Writer/LibPng)
include_directories(${LIBS_PATH}/PDF-Writer/ZLib)
include_directories(${LIBS_PATH}/PDF-Writer/FreeType/include)

file(GLOB headers *.h)
file(GLOB sources *.cc)

add_library (PngWriter STATIC ${headers} ${sources})

target_link_libraries(PngWriter LibPng)
target_link_libraries(PngWriter FreeType)

set_target_properties (PngWriter PROPERTIES FOLDER "00.Libs")