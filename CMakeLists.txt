cmake_minimum_required(VERSION 3.19...3.29)
project(testTask)

set(CMAKE_CXX_STANDARD 17)
#TODO: ConanGen

find_package(Boost 1.86.0 REQUIRED COMPONENTS system filesystem)

file(GLOB_RECURSE SOURCES sources/*.cpp)
include_directories(headers)

add_executable(testTask ${SOURCES}
)

target_link_libraries(testTask PRIVATE Boost::system Boost::filesystem)
