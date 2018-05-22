cmake_minimum_required (VERSION 3.7.0) 
project(problem_9a)

add_definitions(-DCURL_STATICLIB)

include_directories(${LIBS_PATH}/nlohmannjson)
include_directories(${LIBS_PATH}/curl/include)
include_directories(${LIBS_PATH}/curlcpp/include)

add_executable(problem_9a main.cpp)

target_link_libraries(problem_9a libcurl)
target_link_libraries(problem_9a curlcpp)

if (UNIX)
   link_directories(${CURLCPP_LIB_LOCATION})
elseif(WIN32) 
   target_link_libraries(problem_9a Crypt32.lib ws2_32.lib winmm.lib wldap32.lib)
endif() 

add_custom_command(TARGET problem_9a PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:problem_9a>/res)

add_custom_command(TARGET problem_9a PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)
                   
set_target_properties (problem_9a PROPERTIES FOLDER "12.NetworkingCloud")