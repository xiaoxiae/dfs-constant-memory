cmake_minimum_required(VERSION 3.17)
project(inline_dfs)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(inline_dfs_run ${SOURCE_FILES})

include_directories(lib)
add_subdirectory(lib)

target_link_libraries(inline_dfs_run lib)

add_subdirectory(tests)
