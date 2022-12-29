cmake_minimum_required(VERSION 3.2)

project(MVC_Editor VERSION 1)

set(PROJECT MVC_Editor)

add_executable(MVC_Editor main.cpp)

set_target_properties(MVC_Editor PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

install(TARGETS MVC_Editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_CONTACT example@example.com)

set(CXXFLAGS  "-g")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

include(CPack)