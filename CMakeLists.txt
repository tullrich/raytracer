cmake_minimum_required (VERSION 2.6)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -lboost_system -lboost_filesystem -lboost_thread -std=c++11")

project (raytracer)


# -------------------------------------------------------------
# Project Specific Variables                                   
# -------------------------------------------------------------
set (RAYTRACER_VERSION_MAJOR 1)
set (RAYTRACER_VERSION_MINOR 0)
set (RAYTRACER_SRCS_DIR src)
set (RAYTRACER_INCLUDE_DIR src)
set (RAYTRACER_CMAKE_DIR cmake)
set (RAYTRACER_DEP_DIR external)

# -------------------------------------------------------------
# Source Tree Variables                       
# -------------------------------------------------------------
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
file(GLOB RAYTRACER_SRCFILES "${RAYTRACER_SRCS_DIR}/*.cpp" "${RAYTRACER_SRCS_DIR}/cgutils/*.cpp")
include_directories("${RAYTRACER_INCLUDE_DIR}")

# -------------------------------------------------------------
# Dependency Variables                              
# -------------------------------------------------------------
set (CGUTILS_DIR ${RAYTRACER_DEP_DIR}/cg-utils)
add_subdirectory(${CGUTILS_DIR})
include_directories("${CGUTILS_DIR}/include")

# -------------------------------------------------------------
# Build                   
# -------------------------------------------------------------
add_executable(raytracer ${RAYTRACER_SRCFILES})
TARGET_LINK_LIBRARIES(raytracer ${LIBS} ${CGUTILS_LIBRARY})
