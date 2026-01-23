cmake_minimum_required(VERSION 3.10)

project(myapp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/line.cpp
    src/main.cpp
    src/simulation.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(deps/raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)