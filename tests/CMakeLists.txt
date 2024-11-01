cmake_minimum_required(VERSION 3.12.1)
#project(evsimpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  UGIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59
)		  
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Library Configuration
link_directories(lib)

#link_directories(../lib)
include_directories(./include)

set(UNIT_TEST ${PROJECT_NAME}_tests)

add_executable(${UNIT_TEST} src/evsimpp.cpp "src/dummy_atomic.cpp")

target_compile_features(${UNIT_TEST} PRIVATE cxx_std_20)

target_link_libraries(${UNIT_TEST} PRIVATE gtest_main)

add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})

include_directories("${gtest_SOURCE_DIR}/include")

set(TEST_COMMAND_LINE_OPTIONS ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> -R "^${UNIT_TEST}$" --output-on-failures)

## Compiler Options
if(MSVC)
    message("MSVC")
    target_link_libraries(${UNIT_TEST} PRIVATE lib/evsimpp) # libbar.so is found in ${CMAKE_SOURCE_DIR}/lib
    include_directories(../evsimpp/include)
    target_compile_options(${UNIT_TEST}
    PUBLIC
        /std:c++latest  # Define options that MSVC may identify 
    )
else() # Clang + GCC
    target_link_libraries(test_app PRIVATE evsimpp) # libbar.so is found in ${CMAKE_SOURCE_DIR}/lib
    target_compile_options(test_app
    PUBLIC
        -std=c++2a -Wall    # Define options that GCC/Clang may identify
    PRIVATE
        -fPIC 
        -fno-rtti 
    )
endif()
