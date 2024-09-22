

#set(CMAKE_UNITY_BUILD ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)

else()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -fno-exceptions -fno-rtti -fomit-frame-pointer -Wall -Wextra -O0")

endif()

# build bar
add_library(destoer "src/destoer.cpp")
target_include_directories(destoer PUBLIC "src/headers")
