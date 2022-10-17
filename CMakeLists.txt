cmake_minimum_required(VERSION 3.2)

project(ip_filter VERSION 1)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set (TESTS_SOURCES
	    tests.cpp)

add_executable(ip_filter_cli main.cpp)
add_library(ip_filter ip.cpp)

set_target_properties(ip_filter_cli ip_filter PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_link_libraries(ip_filter_cli PRIVATE
    ip_filter
)

if (TESTS_SOURCES)
	set (Boost_USE_STATIC_LIBS ON)
	set (Boost_USE_MULTITHREADED ON)
	find_package (Boost
		COMPONENTS
			unit_test_framework
		REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	set (TEST test_ip_filter)
	add_executable (${TEST} ${TESTS_SOURCES})
	target_link_libraries (${TEST} ip_filter ${Boost_LIBRARIES})
	enable_testing ()
	add_test (${TEST} ${TEST})
endif ()

install(TARGETS ip_filter_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_CONTACT example@example.com)

set(CXXFLAGS  "-g")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

include(CPack)

enable_testing()