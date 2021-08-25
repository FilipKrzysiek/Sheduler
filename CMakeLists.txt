cmake_minimum_required(VERSION 3.16)
project(Scheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../LogLib/resources)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Scheduler/resources)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(includes/Scheduler)

add_library(Scheduler STATIC
        src/Scheduler.cpp
        src/TaskRepeatable.cpp
        src/TaskStaticTime.cpp
        src/SchedulerQueue.cpp
        src/Task.cpp
        src/TaskClass.cpp
        src/TaskFunction.cpp)


add_library(SchedulerShared SHARED
        src/Scheduler.cpp
        src/TaskRepeatable.cpp
        src/TaskStaticTime.cpp
        src/SchedulerQueue.cpp
        src/Task.cpp
        src/TaskClass.cpp
        src/TaskFunction.cpp)

target_link_libraries(Scheduler PRIVATE Threads::Threads)
target_link_libraries(SchedulerShared PRIVATE Threads::Threads)


set_target_properties(Scheduler PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Scheduler/resources)
set_target_properties(SchedulerShared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Scheduler/resources)
set_target_properties(SchedulerShared PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Scheduler/resources)


