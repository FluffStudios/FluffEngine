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
target_compile_definitions(Sandbox PUBLIC 
	"$<$<CONFIG:Release>:O2>" 
	"$<$<CONFIG:Release>:NDEBUG>"
)

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

target_link_libraries( Sandbox Core )

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