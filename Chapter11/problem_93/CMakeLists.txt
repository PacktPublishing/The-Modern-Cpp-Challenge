cmake_minimum_required (VERSION 3.7.0) 
project(problem_93)

include_directories(${LIBS_PATH}/cryptopp)

if(BOOST_FILESYSTEM)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_FILESYSTEM)
else()
   if(MSVC)
      add_definitions(-DFILESYSTEM_EXPERIMENTAL)
   endif()
endif()

add_definitions(-DCRYPTOPP_CXX11_LAMBDA)

add_executable(problem_93 main.cpp)

if(BOOST_FILESYSTEM)
   if(UNIX)
      find_package(Boost COMPONENTS system filesystem REQUIRED)
      
      target_link_libraries(problem_93 ${Boost_FILESYSTEM_LIBRARY}
                                       ${Boost_SYSTEM_LIBRARY})
   endif()
endif()

target_link_libraries(problem_93 cryptlib)

add_custom_command(TARGET problem_93 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/sample.txt $<TARGET_FILE_DIR:problem_93>)
                   
add_custom_command(TARGET problem_93 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/sample.txt ${CMAKE_CURRENT_BINARY_DIR})
                   
set_target_properties (problem_93 PROPERTIES FOLDER "11.Cryptography")