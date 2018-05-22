cmake_minimum_required (VERSION 3.7.0) 
project(problem_70)

include_directories(${LIBS_PATH}/pugixml)
include_directories(${INCLUDE_PATH}/)

if(BOOST_OPTIONAL)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_OPTIONAL)
endif()

add_executable(problem_73 ${LIBS_PATH}/pugixml/pugixml.cpp main.cpp)

set_target_properties (problem_73 PROPERTIES FOLDER "09.DataSerialization")