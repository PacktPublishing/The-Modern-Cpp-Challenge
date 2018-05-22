cmake_minimum_required (VERSION 3.7.0) 
project(problem_87)

include_directories(${LIBS_PATH}/sqlite)
include_directories(${LIBS_PATH}/sqlite_modern_cpp)
include_directories(${INCLUDE_PATH})

if(BOOST_FILESYSTEM)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_FILESYSTEM)
else()
   if(MSVC)
      add_definitions(-DFILESYSTEM_EXPERIMENTAL)
   endif()
endif()

if(BOOST_OPTIONAL)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_OPTIONAL)
endif()

add_executable(problem_87 ${LIBS_PATH}/sqlite/sqlite3.c main.cpp)

if(BOOST_FILESYSTEM)
   if(UNIX)
      find_package(Boost COMPONENTS system filesystem REQUIRED)
      
      target_link_libraries(problem_87 ${Boost_FILESYSTEM_LIBRARY}
                                       ${Boost_SYSTEM_LIBRARY})
   endif()
endif()

add_custom_command(TARGET problem_87 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/cppchallenger87.db $<TARGET_FILE_DIR:problem_87>)
                   
add_custom_command(TARGET problem_87 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/cppchallenger87.db ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET problem_87 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:problem_87>/res)

add_custom_command(TARGET problem_87 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)
                   
set_target_properties (problem_87 PROPERTIES FOLDER "10.ArchivesImagesDatabases")