cmake_minimum_required(VERSION 3.12.1)
project(evsimpp)

# Library Configuration
#link_directories(lib)

# Source Code Configuration
include_directories(./include)
file(GLOB FILE_LISTS src/*)
set(SRCS "")

foreach(FILE_LIST ${FILE_LISTS})
    list(APPEND SRCS ${FILE_LIST})  
endforeach()

message("Source Files : ${SRCS}")

# Compile & Linking Configuration
add_library(lib${PROJECT_NAME} STATIC ${SRCS})
#target_link_libraries(${PROJECT_NAME} bar) # libbar.so is found in ${CMAKE_SOURCE_DIR}/lib

## Compiler Options
if(MSVC)
    target_compile_options(lib${PROJECT_NAME} 
    PUBLIC
        /std:c++latest  # Define options that MSVC may identify 
    )
else() # Clang + GCC
    target_compile_options(lib${PROJECT_NAME}
    PUBLIC
        -std=c++2a -Wall    # Define options that GCC/Clang may identify
    PRIVATE
        -fPIC 
        -fno-rtti 
    )
endif()

# Resource Loading
#FILE(COPY resources/test.xml DESTINATION "${CMAKE_BINARY_DIR}")