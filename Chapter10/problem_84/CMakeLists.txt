cmake_minimum_required (VERSION 3.7.0) 
project(problem_84)

include_directories(${LIBS_PATH}/PDF-Writer/LibPng)
include_directories(${LIBS_PATH}/PDF-Writer/ZLib)
include_directories(${LIBS_PATH}/PDF-Writer/FreeType/include)
include_directories(${LIBS_PATH}/pngwriter)

add_executable(problem_84 main.cpp)

target_link_libraries(problem_84 LibPng)
target_link_libraries(problem_84 Zlib)
target_link_libraries(problem_84 FreeType)
target_link_libraries(problem_84 PngWriter)

set_target_properties (problem_84 PROPERTIES FOLDER "10.ArchivesImagesDatabases")