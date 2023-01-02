#project config

cmake_minimum_required(VERSION 3.10)

project(destoer-test)

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB src_files
	"../src/*.h"
	"../src/main.cpp"
    test.cpp
)

if(WIN32)

else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -fno-exceptions -fmax-errors=8 -fno-rtti  -fomit-frame-pointer -Wall -Wextra -O0")
endif()

include_directories("../src/")
add_executable(destoer-test ${src_files})