SET(_PF86 "PROGRAMFILES(X86)")
SET( IMGUI_SEARCH_PATHS
	./Engine/Dependencies/imgui
)

FIND_PATH( IMGUI_INCLUDE_DIRS
	NAMES
		imgui/imgui.h
	PATHS
		${IMGUI_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where imgui.h resides"
)

FIND_PATH ( IMGUI_SRC_DIRS
    NAMES
        imgui/imgui.cpp
    PATHS
        ${IMGUI_SEARCH_PATHS}
    PATH_SUFFIXES
        src
    DOC
        "The directory where imgui.cpp resides"
)

file ( GLOB_RECURSE IMGUI_SRCS 
    ${IMGUI_SRC_DIRS}/*.cpp
)

# Check if we found it!
IF ( IMGUI_INCLUDE_DIRS )
	SET( IMGUI_FOUND TRUE )
	MESSAGE( STATUS "Looking for IMGUI - found" )
ELSE ( IMGUI_INCLUDE_DIRS )
	SET( IMGUI_FOUND FALSE )
	MESSAGE( STATUS "Looking for IMGUI - not found" )
ENDIF ( IMGUI_INCLUDE_DIRS )