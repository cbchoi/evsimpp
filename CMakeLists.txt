cmake_minimum_required(VERSION 3.12.1)
project(evsimpp)

add_subdirectory(evsimpp)
add_subdirectory(tests)

# Resource Loading
FILE(COPY resources/test.xml DESTINATION "${CMAKE_BINARY_DIR}")