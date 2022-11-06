cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 1)

set(PROJECT allocator)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# set(TESTS_SOURCES
# 	    tests/tests.cpp)

add_executable(allocator_cli main.cpp)

set_target_properties(allocator_cli PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# if (TESTS_SOURCES)
# 	set (Boost_USE_STATIC_LIBS ON)
# 	set (Boost_USE_MULTITHREADED ON)
# 	find_package (Boost
# 		COMPONENTS
# 			unit_test_framework
# 		REQUIRED)
# 	include_directories(${Boost_INCLUDE_DIRS})
# 	set (TEST test_${PROJECT})
# 	add_executable (${TEST} ${TESTS_SOURCES})
# 	target_link_libraries (${TEST} ${PROJECT} ${Boost_LIBRARIES})
# 	enable_testing ()
# 	add_test (${TEST} ${TEST})
# endif ()

install(TARGETS allocator_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CXXFLAGS  "-g")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

include(CPack)

enable_testing()