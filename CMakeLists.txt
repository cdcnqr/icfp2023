cmake_minimum_required(VERSION 3.18)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ICFP2023 VERSION 1.0 DESCRIPTION "ICFP2023" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories(lib)
link_directories(lib)

add_executable(icfp2023 
  src/solver1.cpp
  src/problem.cpp
  src/solution.cpp
  src/optimizer_state.cpp
  src/optimizer.cpp
  src/cgal_calc.cpp
  src/greedy_optimizer.cpp
  src/visualization.cpp
)

add_executable(icfp2023_solver2
  src/solver2.cpp
  src/problem.cpp
  src/optimizer.cpp
  src/optimizer_state.cpp
  src/visualization.cpp
)

add_executable(score_calc_test
  test/score_calc_test.cpp
  src/optimizer.cpp
  src/optimizer_state.cpp
)

add_compile_options($<$<CONFIG:Debug>:-gdwarf-4> $<$<CONFIG:Release>:-O3>)

find_package(CGAL REQUIRED COMPONENTS Core)
target_link_libraries(score_calc_test PRIVATE CGAL::CGAL CGAL::CGAL_Core)
target_link_libraries(icfp2023 sfml-graphics sfml-window sfml-system CGAL::CGAL CGAL::CGAL_Core)
target_link_libraries(icfp2023_solver2 PRIVATE CGAL::CGAL CGAL::CGAL_Core sfml-graphics sfml-window sfml-system)

# sudo apt-get install libsfml-dev

# Configure debug build
# cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug

# Configure release build
# cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Release

# Actual Build
# cmake --build build/
