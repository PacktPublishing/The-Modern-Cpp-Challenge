cmake_minimum_required (VERSION 3.7.0) 
project(problem_97)

add_definitions(-DCURL_STATICLIB)

include_directories(${LIBS_PATH}/nlohmannjson)
include_directories(${LIBS_PATH}/curl/include)
include_directories(${LIBS_PATH}/curlcpp/include)

add_executable(problem_97 main.cpp)

target_link_libraries(problem_97 libcurl)
target_link_libraries(problem_97 curlcpp)

if (UNIX)
   link_directories(${CURLCPP_LIB_LOCATION})
elseif(WIN32) 
   target_link_libraries(problem_97 Crypt32.lib ws2_32.lib winmm.lib wldap32.lib)
endif()   

set_target_properties (problem_97 PROPERTIES FOLDER "12.NetworkingCloud")