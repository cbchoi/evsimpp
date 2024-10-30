cmake_minimum_required(VERSION 3.12.1)
project(model_ext)

# Update latest header file
FILE(COPY "../include/agent.hpp" DESTINATION "${PROJECT_SOURCE_DIR}/include")

include_directories(./include)
file(GLOB FILE_LISTS src/*)
set(SRCS "")

foreach(FILE_LIST ${FILE_LISTS})
    list(APPEND SRCS ${FILE_LIST})  
endforeach()

message("Source Files : ${SRCS}")

# Compile & Linking Configuration
add_library(${PROJECT_NAME} ${SRCS})

## Compiler Options
if(MSVC)
    target_compile_options(${PROJECT_NAME} 
    PUBLIC
        /std:c++latest  # Define options that MSVC may identify 
    )
else() # Clang + GCC
    target_compile_options(${PROJECT_NAME}
    PUBLIC
        -std=c++2a -Wall    # Define options that GCC/Clang may identify
    PRIVATE
        -fPIC 
        -fno-rtti 
    )
endif()