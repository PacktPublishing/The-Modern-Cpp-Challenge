cmake_minimum_required (VERSION 3.7.0) 
project(problem_74)

include_directories(${LIBS_PATH}/pugixml)

add_executable(problem_74 ${LIBS_PATH}/pugixml/pugixml.cpp main.cpp)

set_target_properties (problem_74 PROPERTIES FOLDER "09.DataSerialization")