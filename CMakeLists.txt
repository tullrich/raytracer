cmake_minimum_required (VERSION 2.6)
project (raytracer)

# versioning
set (Raytracer_VERSION_MAJOR 1)
set (Raytracer_VERSION_MINOR 0)

# directory vars
set (Raytracer_SRCS_DIR src)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
file(GLOB _srcFiles "${Raytracer_SRCS_DIR}/*.cpp")

# search path
include_directories("${PROJECT_BINARY_DIR}")

add_executable(raytracer ${_srcFiles})
