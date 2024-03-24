cmake_minimum_required(VERSION 3.16)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(scheduler_example src/main.cpp)
target_link_libraries(scheduler_example PRIVATE scheduler)