cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 11)
project(KVMap)

#add_library(istache ${CMAKE_CURRENT_SOURCE_DIR}/pistache)
#add_library(istache ./pistache)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pistache/include)
#add_subdirectory(./pistache)
message(STATUS "==================")
message(STATUS $ENV{CMAKE_MODULE_PATH})
message(STATUS ${CMAKE_MODULE_PATH})
#                 CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "/Users/edsonticonazegarra/restbed/distribution/include/restbed /Users/edsonticonazegarra/restbed/distribution/include/corvusoft/restbed /Users/edsonticonazegarra/restbed/distribution/library ")
set(ENV{CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})
message(STATUS $ENV{CMAKE_MODULE_PATH})
message(STATUS ${CMAKE_MODULE_PATH})
message(STATUS "==================")
#find_package(restbed REQUIRED)


include_directories("/Users/edsonticonazegarra/restbed/distribution/include/")
#link_directories("/Users/edsonticonazegarra/restbed/build")
find_library(RESTBED_LIBRARY restbed HINTS /Users/edsonticonazegarra/restbed/build-4.7)
#target_link_libraries(KVMap PUBLIC "/Users/edsonticonazegarra/restbed/build")

#include_directories(${restbed_INCLUDE_DIR})


#https://stackoverflow.com/questions/31438916/cmake-cannot-find-library-using-link-directories/31471772?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa

add_executable(KVMap main.cpp kvmap.cpp)
#target_link_libraries(KVMAp pistache)
target_link_libraries(KVMap PUBLIC ${RESTBED_LIBRARY})
message(STATUS ${RESTBED_LIBRARY})
message(STATUS "==================")
