cmake_minimum_required(VERSION 3.26)
project(COP4533)


#set standard enviroemnt
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#show possible errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


#setup catch2 tests
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

include_directories(src)


add_subdirectory(rapidcheck)
#include_directories(rapidcheck/extras/gtest/include)

add_executable(runMain src/a.cpp)



add_executable(Tests
        test/test.cpp # your test file
        # add your own header files below
)



target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain rapidcheck INTERFACE rapidcheck_catch)
target_include_directories(Tests PUBLIC rapidcheck/extras/catch/include)



# comment everything below out if you are using CLion
include(CTest)
include(Catch)
catch_discover_tests(Tests) # must be named the same as your test executable