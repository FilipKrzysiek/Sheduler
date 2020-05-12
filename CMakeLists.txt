cmake_minimum_required(VERSION 3.15)
project(Sheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Sheduler/resources)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../LogLib/resources)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

add_library(Sheduler STATIC src/Sheduler.cpp includes/Sheduler.h src/TaskRepeatable.cpp includes/TaskRepeatable.h src/TaskStaticTime.cpp includes/TaskStaticTime.h)


