cmake_minimum_required(VERSION 3.10)

project(myapp LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(myapp
    src/main.cpp
)

target_include_directories(myapp PRIVATE include)

add_subdirectory(deps/raylib)
target_link_libraries(myapp PRIVATE raylib)