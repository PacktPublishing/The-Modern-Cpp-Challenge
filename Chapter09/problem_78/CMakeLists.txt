cmake_minimum_required (VERSION 3.7.0) 
project(problem_78)

include_directories(${LIBS_PATH}/PDF-Writer)
include_directories(${LIBS_PATH}/PDF-Writer/FreeType/include)

if(BOOST_FILESYSTEM)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_FILESYSTEM)
else()
   if(MSVC)
      add_definitions(-DFILESYSTEM_EXPERIMENTAL)
   endif()
endif()

add_executable(problem_78 main.cpp)

if(BOOST_FILESYSTEM)
   if(UNIX)
      find_package(Boost COMPONENTS system filesystem REQUIRED)
      
      target_link_libraries(problem_78 ${Boost_FILESYSTEM_LIBRARY}
                                       ${Boost_SYSTEM_LIBRARY})
   endif()
endif()

target_link_libraries(problem_78 ZipLib)
target_link_libraries(problem_78 PDFWriter)
target_link_libraries(problem_78 FreeType)
target_link_libraries(problem_78 LibAesgm)
target_link_libraries(problem_78 LibJpeg)
target_link_libraries(problem_78 LibPng)
target_link_libraries(problem_78 LibTiff)
target_link_libraries(problem_78 Zlib)

add_custom_command(TARGET problem_78 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:problem_78>/res)
                   
add_custom_command(TARGET problem_78 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)
                   
set_target_properties (problem_78 PROPERTIES FOLDER "09.DataSerialization")                   