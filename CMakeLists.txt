cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Cinnabar VERSION 1.0 DESCRIPTION "ICFP2023" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories(lib)
link_directories(lib)

file(GLOB_RECURSE SOURCES "src/*.cpp" main.cpp)

add_executable(icfp2023 ${SOURCES})

add_compile_options($<$<CONFIG:Debug>:-gdwarf-4> $<$<CONFIG:Release>:-O3>)

target_link_libraries(icfp2023 sfml-graphics sfml-window sfml-system)

# Configure debug build
# cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug

# Configure release build
# cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Release

# Actual Build
# cmake --build build/
