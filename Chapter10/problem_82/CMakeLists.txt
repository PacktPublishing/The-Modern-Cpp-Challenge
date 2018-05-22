cmake_minimum_required (VERSION 3.7.0) 
project(problem_82)

include_directories(${LIBS_PATH}/PDF-Writer/LibPng)
include_directories(${LIBS_PATH}/PDF-Writer/ZLib)
include_directories(${LIBS_PATH}/PDF-Writer/FreeType/include)
include_directories(${LIBS_PATH}/pngwriter)

add_executable(problem_82 main.cpp)

target_link_libraries(problem_82 LibPng)
target_link_libraries(problem_82 Zlib)
target_link_libraries(problem_82 FreeType)
target_link_libraries(problem_82 PngWriter)

set_target_properties (problem_82 PROPERTIES FOLDER "10.ArchivesImagesDatabases")