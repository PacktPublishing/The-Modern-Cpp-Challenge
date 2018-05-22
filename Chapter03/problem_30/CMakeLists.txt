cmake_minimum_required (VERSION 3.7.0) 
project(problem_30)

if(BOOST_OPTIONAL)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_OPTIONAL)
endif()

add_executable(problem_30 main.cpp)

set_target_properties (problem_30 PROPERTIES FOLDER "03.Strings")