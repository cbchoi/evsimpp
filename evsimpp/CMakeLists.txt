cmake_minimum_required(VERSION 3.12.1)
#project(evsimpp)

message("Configuring evsim++")

# Library Configuration
#link_directories(lib)

# Source Code Configuration
include_directories(include)
file(GLOB FILE_LISTS src/*)
set(SRCS "")

foreach(FILE_LIST ${FILE_LISTS})
    list(APPEND SRCS ${FILE_LIST})  
endforeach()

message("Source Files : ${SRCS}")

#target_link_libraries(${PROJECT_NAME} bar) # libbar.so is found in ${CMAKE_SOURCE_DIR}/lib

## Compiler Options
if(MSVC)
# Compile & Linking Configuration
    add_library(evsimpp STATIC ${SRCS})
    target_include_directories(evsimpp PUBLIC  include)
    target_link_libraries(evsimpp PUBLIC lib)

    target_compile_options(evsimpp 
    PUBLIC
        /std:c++latest  # Define options that MSVC may identify 
    )
else() # Clang + GCC
    add_library(evsimpp STATIC ${SRCS})
    target_include_directories(evsimpp PUBLIC include)
    #target_link_libraries(${PROJECT_NAME} PUBLIC lib)

    target_compile_options(evsimpp
    PUBLIC
        -std=c++2a -Wall    # Define options that GCC/Clang may identify
    PRIVATE
        -fPIC 
        -fno-rtti 
    )
endif()

# Resource Loading
#FILE(COPY resources/test.xml DESTINATION "${CMAKE_BINARY_DIR}")
