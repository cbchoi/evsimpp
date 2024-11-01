cmake_minimum_required(VERSION 3.12.1)
project(evsimpp)

# GoogleTest requires at least C+204
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )

add_subdirectory(evsimpp)

# setup testing
include(CTest)
enable_testing()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

include_directories("${gtest_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include"
)

add_subdirectory(tests)

# Resource Loading
FILE(COPY resources/test.xml DESTINATION "${CMAKE_BINARY_DIR}")