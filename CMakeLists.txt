cmake_minimum_required(VERSION 3.10)

project(myapp LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/line.c
    src/list.c
    src/main.c
    src/simulation.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(deps/raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)