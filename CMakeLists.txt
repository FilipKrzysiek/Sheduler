cmake_minimum_required(VERSION 3.16)
project(scheduler VERSION 0.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(scheduler STATIC
        src/Scheduler.cpp include/Scheduler/Scheduler.h
        src/TaskClass.cpp include/Scheduler/TaskClass.h
        src/TaskController.cpp include/Scheduler/TaskController.h
        src/TaskFunction.cpp include/Scheduler/TaskFunction.h
        src/TaskRepeatable.cpp include/Scheduler/TaskRepeatable.h
        src/TaskStaticTime.cpp include/Scheduler/TaskStaticTime.h
        include/Scheduler/TaskBind.cpp
        include/Scheduler/TaskBind.h
)


add_library(schedulerShared SHARED
        src/Scheduler.cpp
        src/TaskClass.cpp
        src/TaskController.cpp
        src/TaskFunction.cpp
        src/TaskRepeatable.cpp
        src/TaskStaticTime.cpp
        include/Scheduler/TaskBind.cpp
        include/Scheduler/TaskBind.h
)

target_link_libraries(scheduler PRIVATE Threads::Threads)
target_include_directories(scheduler PRIVATE include/Scheduler)

target_link_libraries(schedulerShared PRIVATE Threads::Threads)
target_include_directories(schedulerShared PRIVATE include/Scheduler)

add_subdirectory(example/onMainThread)
add_subdirectory(example/onSeparatedThread)

#add_executable(scheduler_example example/src/main.cpp)
#target_link_libraries(scheduler_example PRIVATE scheduler)
#target_include_directories(scheduler_example PRIVATE include)