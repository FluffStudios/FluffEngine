cmake_minimum_required(VERSION 3.6)

project(FluffEngine)

option(BUILD_ENGINE "Generate engine project" ON)
option(BUILD_SANDBOX "Generate sandbox project" ON)

# We need a CMAKE_DIR with some code to find external dependencies
SET(FluffEngine_CMAKE_DIR "${FluffEngine_SOURCE_DIR}/CMake")

if(BUILD_ENGINE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Engine)
endif()

if(BUILD_SANDBOX)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Sandbox)
endif()