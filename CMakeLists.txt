cmake_minimum_required(VERSION 3.28)
project(untitled12)
include(fetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled12 main.cpp
        main.cpp)
target_link_libraries(untitled12 sfml-graphics sfml-audio)