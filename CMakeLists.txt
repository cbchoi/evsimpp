cmake_minimum_required(VERSION 3.12.1)
project(evsimpp)

add_compile_options(-std=c++11)

include_directories(./include)
file(GLOB FILE_LISTS src/*)
set(SRCS "")

foreach(FILE_LIST ${FILE_LISTS})
    list(APPEND SRCS ${FILE_LIST})  
endforeach()

message("ADD SRC : ${SRCS}")

add_executable(${PROJECT_NAME} ${SRCS})
