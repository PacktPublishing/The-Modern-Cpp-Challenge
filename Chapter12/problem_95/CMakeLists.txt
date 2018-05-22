cmake_minimum_required (VERSION 3.7.0) 
project(problem_95)

if (UNIX)
else()
  add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_WIN32_WINNT=0x0601)
endif()

include_directories(${LIBS_PATH}/asio/include)

add_executable(problem_95 main.cpp)

set_target_properties (problem_95 PROPERTIES FOLDER "12.NetworkingCloud")