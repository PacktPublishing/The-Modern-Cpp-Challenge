cmake_minimum_required (VERSION 3.7.0) 
project(problem_77)

include_directories(${LIBS_PATH}/PDF-Writer)
include_directories(${LIBS_PATH}/PDF-Writer/FreeType/include)
include_directories(${INCLUDE_PATH})

if(BOOST_OPTIONAL)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_OPTIONAL)
endif()

add_executable(problem_77 main.cpp)

target_link_libraries(problem_77 ZipLib)
target_link_libraries(problem_77 PDFWriter)
target_link_libraries(problem_77 FreeType)
target_link_libraries(problem_77 LibAesgm)
target_link_libraries(problem_77 LibJpeg)
target_link_libraries(problem_77 LibPng)
target_link_libraries(problem_77 LibTiff)
target_link_libraries(problem_77 Zlib)

set_target_properties (problem_77 PROPERTIES FOLDER "09.DataSerialization")