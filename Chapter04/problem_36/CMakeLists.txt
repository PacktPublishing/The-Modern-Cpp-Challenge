cmake_minimum_required (VERSION 3.7.0) 
project(problem_36)

if(BOOST_FILESYSTEM)
   include_directories(${BOOST_INCLUDE_DIR})
   link_directories(${BOOST_LIB_DIR})
   add_definitions(-DUSE_BOOST_FILESYSTEM)
else()
   if(MSVC)
      add_definitions(-DFILESYSTEM_EXPERIMENTAL)
   endif()
endif()

add_executable(problem_36 main.cpp)

if(BOOST_FILESYSTEM)
   if(UNIX)
      find_package(Boost COMPONENTS system filesystem REQUIRED)
      
      target_link_libraries(problem_36 ${Boost_FILESYSTEM_LIBRARY}
                                       ${Boost_SYSTEM_LIBRARY})
   endif()
endif()

set_target_properties (problem_36 PROPERTIES FOLDER "04.Streams")