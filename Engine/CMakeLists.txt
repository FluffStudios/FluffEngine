# add the -c and -Wall flags
if(MSVC)
	add_definitions(
		-c
		-W4
	)
else()
	add_definitions(
		-c
		-Wall
	)
endif()

add_definitions( -D_CRT_SECURE_NO_WARNINGS )
add_definitions( -DGLEW_STATIC )

# Lets LOAD up our headers for the core project
file(GLOB_RECURSE CORE_HDRS
	${FluffEngine_SOURCE_DIR}/Engine/Source/Core/Public/*.h
)

# Lets LOAD up our sources for the core project
file(GLOB_RECURSE CORE_SRCS
	${FluffEngine_SOURCE_DIR}/Engine/Source/Core/Private/*.cpp
	${FluffEngine_SOURCE_DIR}/Engine/Source/Core/Private/*.c
)

include_directories(${FluffEngine_SOURCE_DIR}/Engine/Source/Core/Public)

# Define the executable
add_library(Core STATIC ${CORE_HDRS} ${CORE_SRCS})

target_compile_definitions(Core PUBLIC 
	"$<$<CONFIG:Release>:O2>" 
	"$<$<CONFIG:Release>:NDEBUG>"
)

#######################################
# LOOK for the packages that we need! #
#######################################

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
INCLUDE(${FluffEngine_CMAKE_DIR}/FindGLEW.cmake)

# GLFW
INCLUDE(${FluffEngine_CMAKE_DIR}/FindGLFW.cmake)

# Assimp
INCLUDE(${FluffEngine_CMAKE_DIR}/FindAssimp.cmake)

# Cereal
INCLUDE(${FluffEngine_CMAKE_DIR}/FindCereal.cmake)

# GLM
INCLUDE(${FluffEngine_CMAKE_DIR}/FindGLM.cmake)

# Physx
INCLUDE(${FluffEngine_CMAKE_DIR}/FindPhysx.cmake)

# STB
INCLUDE(${FluffEngine_CMAKE_DIR}/FindSTB.cmake)

# Define the include DIRs
include_directories(
	${FluffEngine_SOURCE_DIR}/Engine/Source
	${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${CEREAL_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${PHYSX_INCLUDE_DIRS}
	${STB_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries( Core
	${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${GLM_LIBRARIES}
    ${PHYSX_LIBRARIES}
)

if(WIN32)
	string(REPLACE "/" "\\" source_path_windows "${FluffEngine_SOURCE_DIR}/Engine/Content")
	string(REPLACE "/" "\\" build_path_windows "${FluffEngine_BINARY_DIR}/Engine/Content")
	execute_process(COMMAND cmd.exe /c mklink /J "${build_path_windows}" "${source_path_windows}" RESULT_VARIABLE exitcode)
else()
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${FluffEngine_SOURCE_DIR}/Engine/Content ${FluffEngine_BINARY_DIR}/Engine/Content RESULT_VARIABLE exitcode)
endif()

if(NOT ${exitcode} EQUAL 0)
	MESSAGE("SYMLINKING FAILED: ${exitcode}")
	MESSAGE("FALLING BACK TO COPYING")
endif()

#Copy file if the build resource folder contains an older version. This is ignored when the symlink was succesful because the files are already the newest version.
file(GLOB_RECURSE RES RELATIVE ${FluffEngine_SOURCE_DIR}/Engine/Content/ ${FluffEngine_SOURCE_DIR}/Engine/Content/*.*)
foreach(file IN LISTS RES)
	configure_file(${FluffEngine_SOURCE_DIR}/Engine/Content/${file} ${FluffEngine_BINARY_DIR}/Engine/Content/${file} COPYONLY)
endforeach()

#Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	foreach(source IN LISTS CORE_SRCS CORE_HDRS)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${FluffEngine_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
		string(REPLACE "Engine\\" "" actual_source_path_msvc "${source_path_msvc}")
		source_group("${actual_source_path_msvc}" FILES "${source}")
	endforeach()
endif()