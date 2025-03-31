cmake_minimum_required(VERSION 3.10)
project(QuadTree)

if(NOT_CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories($(PROJECT_SOURCE_DIR)/include)
add_executable(mesh_quadtree mesh_quadtree.cpp
                        src/GeometryBoundary.cpp
                        src/QuadTree.cpp
                        src/utils.cpp)

target_compile_definitions(mesh_quadtree PUBLIC PROJECT_DIR="${CMAKE_SOURCE_DIR}")
