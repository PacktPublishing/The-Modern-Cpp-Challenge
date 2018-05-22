cmake_minimum_required (VERSION 3.7.0) 
project(problem_42)

include_directories(${LIBS_PATH}/date/include/date)

add_executable(problem_42 main.cpp)

set_target_properties (problem_42 PROPERTIES FOLDER "05.Datetime")