cmake_minimum_required(VERSION 3.13.0)
project(eda_rb_tree VERSION 0.1.0)

include(CTest)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE Generator: ${CMAKE_GENERATOR}")
message(STATUS "Build Test: ${PLL_ENABLE_BUILD_TESTING}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/x64/)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/x64/)

if (NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-function -Wno-unused-parameter")
endif()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
add_library(eda_rb_tree STATIC ${SOURCES})
target_include_directories(eda_rb_tree PUBLIC ${PROJECT_SOURCE_DIR}/include)

enable_testing()

add_executable(rb_tree_test "test/rb_tree_test.c")
target_link_libraries(rb_tree_test PRIVATE eda_rb_tree)
add_test(NAME rb_tree_test COMMAND rb_tree_test)
target_include_directories(rb_tree_test PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_target_properties(eda_rb_tree PROPERTIES
	C_STANDARD 11
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)

set_target_properties(rb_tree_test PROPERTIES
	C_STANDARD 11
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)
