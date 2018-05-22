cmake_minimum_required (VERSION 3.7.0) 
project(problem_41)

include_directories(${LIBS_PATH}/date/include/date)

add_executable(problem_41 main.cpp)

set_target_properties (problem_41 PROPERTIES FOLDER "05.Datetime")