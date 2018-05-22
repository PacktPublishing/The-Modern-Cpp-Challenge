cmake_minimum_required (VERSION 3.7.0) 
project(problem_91)

include_directories(${LIBS_PATH}/cryptopp)
add_definitions(-DCRYPTOPP_CXX11_LAMBDA)

add_executable(problem_91 main.cpp)

target_link_libraries(problem_91 cryptlib)

set_target_properties (problem_91 PROPERTIES FOLDER "11.Cryptography")