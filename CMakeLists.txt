cmake_minimum_required(VERSION 3.2)

project(MVC-Editor VERSION 1)

set(PROJECT MVC-Editor )

add_executable(MVC-Editor main.cpp)

set_target_properties(MVC-Editor PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)