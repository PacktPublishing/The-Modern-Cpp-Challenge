cmake_minimum_required (VERSION 3.7.0) 
project(problem_79)

include_directories(${LIBS_PATH}/ZipLib)

if(BOOST_FILESYSTEM)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_FILESYSTEM)
else()
   if(MSVC)
      add_definitions(-DFILESYSTEM_EXPERIMENTAL)
   endif()
endif()

add_executable(problem_79 main.cpp)

if(BOOST_FILESYSTEM)
   if(UNIX)
      find_package(Boost COMPONENTS system filesystem REQUIRED)
      
      target_link_libraries(problem_79 ${Boost_FILESYSTEM_LIBRARY}
                                       ${Boost_SYSTEM_LIBRARY})
   endif()
endif()

target_link_libraries(problem_79 ZipLib)

add_custom_command(TARGET problem_79 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/sample79.zip $<TARGET_FILE_DIR:problem_79>)
                   
add_custom_command(TARGET problem_79 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/sample79.zip ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties (problem_79 PROPERTIES FOLDER "10.ArchivesImagesDatabases")