cmake_minimum_required (VERSION 3.7.0) 
project(problem_40)

include_directories(${LIBS_PATH}/date/include/date)

add_executable(problem_40 main.cpp)

set_target_properties (problem_40 PROPERTIES FOLDER "05.Datetime")