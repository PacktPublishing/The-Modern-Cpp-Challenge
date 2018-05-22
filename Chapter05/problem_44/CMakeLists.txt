cmake_minimum_required (VERSION 3.7.0) 
project(problem_44)

include_directories(${LIBS_PATH}/date/include/date)

add_executable(problem_44 main.cpp)

set_target_properties (problem_44 PROPERTIES FOLDER "05.Datetime")