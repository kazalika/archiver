project(Archiver)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(library)
add_subdirectory(tests)

add_executable(archiver main.cpp)

target_link_libraries(archiver archiver_library)