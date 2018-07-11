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

if ( NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(
		CMAKE_BUILD_TYPE
			"Debug"
		CACHE STRING
			"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
	)
endif ( NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")

add_definitions( -D_CRT_SECURE_NO_WARNINGS )
add_definitions( -DGLEW_STATIC )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
add_definitions( -O2 )
add_definitions( -NDEBUG )
endif ( CMAKE_BUILD_TYPE STREQUAL "Release" )

file(GLOB_RECURSE SANDBOX_HDRS
	${FluffEngine_SOURCE_DIR}/Sandbox/Source/Public/*.h
)

file(GLOB_RECURSE SANDBOX_SRCS
	${FluffEngine_SOURCE_DIR}/Sandbox/Source/Private/*.cpp
	${FluffEngine_SOURCE_DIR}/Sandbox/Source/Private/*.c
)

include_directories(${FluffEngine_SOURCE_DIR}/Sandbox/Source/Public)

# Define the executable
add_executable(Sandbox ${SANDBOX_HDRS} ${SANDBOX_SRCS})

# Define the include DIRs
include_directories(
	${FluffEngine_SOURCE_DIR}/Engine/Source/Core/Public
	${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${CEREAL_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${PHYSX_INCLUDE_DIRS}
	${STB_INCLUDE_DIRS}
)

target_link_libraries( Sandbox
    Core
	# ${OPENGL_LIBRARIES}
    # ${GLEW_LIBRARIES}
    # ${GLFW_LIBRARIES}
    # ${ASSIMP_LIBRARIES}
    # ${GLM_LIBRARIES}
    # ${PHYSX_LIBRARIES}
)

if(WIN32)
	string(REPLACE "/" "\\" source_path_windows "${FluffEngine_SOURCE_DIR}/Sandbox/Content")
	string(REPLACE "/" "\\" build_path_windows "${FluffEngine_BINARY_DIR}/Sandbox/Content")
	execute_process(COMMAND cmd.exe /c mklink /J "${build_path_windows}" "${source_path_windows}" RESULT_VARIABLE exitcode)
else()
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${FluffEngine_SOURCE_DIR}/Sandbox/Content ${FluffEngine_BINARY_DIR}/Sandbox/Content RESULT_VARIABLE exitcode)
endif()

if(NOT ${exitcode} EQUAL 0)
	MESSAGE("SYMLINKING FAILED: ${exitcode}")
	MESSAGE("FALLING BACK TO COPYING")
endif()

#Copy file if the build resource folder contains an older version. This is ignored when the symlink was succesful because the files are already the newest version.
file(GLOB_RECURSE RES RELATIVE ${FluffEngine_SOURCE_DIR}/Sandbox/Content/ ${FluffEngine_SOURCE_DIR}/Sandbox/Content/*.*)
foreach(file IN LISTS RES)
	configure_file(${FluffEngine_SOURCE_DIR}/Sandbox/Content/${file} ${FluffEngine_BINARY_DIR}/Sandbox/Content/${file} COPYONLY)
endforeach()

#Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	foreach(source IN LISTS SANDBOX_SRCS SANDBOX_HDRS)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${FluffEngine_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
		source_group("${actual_source_path_msvc}" FILES "${source}")
	endforeach()
endif()