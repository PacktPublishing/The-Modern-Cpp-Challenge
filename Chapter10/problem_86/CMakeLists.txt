cmake_minimum_required (VERSION 3.7.0) 
project(problem_86)

include_directories(${LIBS_PATH}/sqlite)
include_directories(${LIBS_PATH}/sqlite_modern_cpp)
include_directories(${INCLUDE_PATH})

if(BOOST_OPTIONAL)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_OPTIONAL)
endif()

add_executable(problem_86 ${LIBS_PATH}/sqlite/sqlite3.c main.cpp)

add_custom_command(TARGET problem_86 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/cppchallenger86.db $<TARGET_FILE_DIR:problem_86>)
                   
add_custom_command(TARGET problem_86 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_SOURCE_DIR}/cppchallenger86.db ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties (problem_86 PROPERTIES FOLDER "10.ArchivesImagesDatabases")