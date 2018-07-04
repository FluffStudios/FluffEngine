SET(_PF86 "PROGRAMFILES(X86)")
SET( GLFW_SEARCH_PATHS
	${GLFW_ROOT_DIR}					# GLFW!
	./Engine/Dependencies/glfw
	$ENV{PROGRAMFILES}/GLFW				# WINDOWS
	"$ENV{_PF86}/GLFW"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( GLFW_INCLUDE_DIRS
	NAMES
        glfw3.h
        glfw3native.h
	PATHS
		${GLFW_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where glfw3.h and glfw3native.h resides"
)

FIND_LIBRARY( GLFW_LIBRARIES
	NAMES
		glfw3-debug glfw3 glfw3dll
	PATHS
		${GLFW_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/Release/Win32
		lib/Release/x64
	DOC
		"The GLFW library"
)

# Check if we found it!
IF ( GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES )
	SET( GLFW_FOUND TRUE )
	MESSAGE( STATUS "Looking for GLFW - found" )
ELSE ( GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES )
	SET( GLFW_FOUND FALSE )
	MESSAGE( STATUS "Looking for GLFW - not found" )
ENDIF ( GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES )