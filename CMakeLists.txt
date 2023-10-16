# cmake_minimum_required(VERSION 3.24.2)
project(
    TapeSort
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(test)
