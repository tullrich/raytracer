cmake_minimum_required (VERSION 2.6)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project (raytracer)


# -------------------------------------------------------------
# Project Specific Variables
# -------------------------------------------------------------
set (RAYTRACER_VERSION_MAJOR 1)
set (RAYTRACER_VERSION_MINOR 0)
set (RAYTRACER_SRCS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (RAYTRACER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (RAYTRACER_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# -------------------------------------------------------------
# Source Tree Variables
# -------------------------------------------------------------
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
file(GLOB RAYTRACER_SRCFILES "${RAYTRACER_SRCS_DIR}/*.cpp" "${RAYTRACER_SRCS_DIR}/*.h")
include_directories("${RAYTRACER_INCLUDE_DIR}")

# -------------------------------------------------------------
# Dependency Variables
# -------------------------------------------------------------
# -------------------------------------------------------------
# FreeImage Dependency Tracking
# -------------------------------------------------------------
INCLUDE( ${RAYTRACER_CMAKE_DIR}/FindFreeImage.cmake )
IF(FREEIMAGE_FOUND)
	SET(HAVE_FREEIMAGE ${FREEIMAGE_FOUND} CACHE BOOL "Set to TRUE if FreeImage is found, FALSE otherwise")
	MESSAGE(STATUS "Looking for FreeImage - found")
	SET(LIBS ${LIBS} ${FREEIMAGE_LIBRARIES})
	INCLUDE_DIRECTORIES(${FREEIMAGE_INCLUDE_PATH})
ELSE(FREEIMAGE_FOUND)
	MESSAGE(FATAL_ERROR "Looking for FreeImage - not found")
ENDIF(FREEIMAGE_FOUND)

# -------------------------------------------------------------
# GLM Dependency Tracking
# -------------------------------------------------------------
INCLUDE( ${RAYTRACER_CMAKE_DIR}/FindGLM.cmake )
IF (GLM_FOUND)
	MESSAGE(STATUS "Looking for GLM - found")
	INCLUDE_DIRECTORIES(${GLM_INCLUDE_DIRS})
ELSE (GLM_FOUND)
	MESSAGE(FATAL_ERROR "Looking for GLM - not found")
ENDIF(GLM_FOUND)

# -------------------------------------------------------------
# Boost Dependency Tracking
# -------------------------------------------------------------
FIND_PACKAGE( Boost COMPONENTS system REQUIRED )
IF (Boost_FOUND)
	MESSAGE(STATUS "Looking for Boost - found")
	SET(LIBS ${LIBS} ${Boost_LIBRARIES})
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ELSE (Boost_FOUND)
	MESSAGE(FATAL_ERROR "Looking for Boost - not found")
ENDIF(Boost_FOUND)

# -------------------------------------------------------------
# Assimp Dependency Tracking
# -------------------------------------------------------------
INCLUDE( ${RAYTRACER_CMAKE_DIR}/FindAssimp.cmake )
IF (ASSIMP_FOUND)
	MESSAGE(STATUS "Looking for Assimp - found")
	SET(LIBS ${LIBS} ${ASSIMP_LIBRARY})
	INCLUDE_DIRECTORIES(${ASSIMP_INCLUDE_DIR})
	set(ASSIMP_ENABLED true)
ELSE (ASSIMP_FOUND)
	MESSAGE(FATAL_ERROR "Looking for Assimp - not found")
ENDIF(ASSIMP_FOUND)

# -------------------------------------------------------------
# Build
# -------------------------------------------------------------
ADD_DEFINITIONS( -D_USE_MATH_DEFINES )
ADD_EXECUTABLE(raytracer ${RAYTRACER_SRCFILES})
TARGET_LINK_LIBRARIES(raytracer ${LIBS})
