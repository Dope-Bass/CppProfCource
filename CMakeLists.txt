cmake_minimum_required(VERSION 3.2)

project(bayan VERSION 1)

set(PROJECT bayan)

find_package( 
 Boost 1.65 REQUIRED 
 COMPONENTS  filesystem program_options 
)
install(PROGRAMS ${Boost_LIBRARIES} DESTINATION ~/install/lib)

add_executable(bayan main.cpp)

TARGET_LINK_LIBRARIES( bayan LINK_PUBLIC ${Boost_LIBRARIES} )

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_CONTACT example@example.com)

set(CXXFLAGS  "-g")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

include(CPack)