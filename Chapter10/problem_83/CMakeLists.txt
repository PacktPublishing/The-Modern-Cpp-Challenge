cmake_minimum_required (VERSION 3.7.0) 
project(problem_83)

include_directories(${LIBS_PATH}/PDF-Writer/LibPng)
include_directories(${LIBS_PATH}/PDF-Writer/ZLib)
include_directories(${LIBS_PATH}/PDF-Writer/FreeType/include)
include_directories(${LIBS_PATH}/pngwriter)

add_executable(problem_83 main.cpp)

target_link_libraries(problem_83 LibPng)
target_link_libraries(problem_83 Zlib)
target_link_libraries(problem_83 FreeType)
target_link_libraries(problem_83 PngWriter)

set_target_properties (problem_83 PROPERTIES FOLDER "10.ArchivesImagesDatabases")