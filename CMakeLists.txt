cmake_minimum_required(VERSION 3.5.0)

project(
    vector
)


set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)

