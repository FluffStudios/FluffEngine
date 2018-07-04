SET(_PF86 "PROGRAMFILES(X86)")
SET( GLEW_SEARCH_PATHS
	${GLEW_ROOT_DIR}					# GLEW!
	./Engine/Dependencies/glew
	$ENV{PROGRAMFILES}/GLEW				# WINDOWS
	"$ENV{_PF86}/GLEW"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( GLEW_INCLUDE_DIRS
	NAMES
		glew.h
	PATHS
		${GLEW_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where glew.h resides"
)

FIND_LIBRARY( GLEW_LIBRARIES
	NAMES
		glew32s
	PATHS
		${GLEW_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/Release/Win32
		lib/Release/x64
	DOC
		"The GLEW library"
)

# Check if we found it!
IF ( GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES )
	SET( GLEW_FOUND TRUE )
	MESSAGE( STATUS "Looking for GLEW - found" )
ELSE ( GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES )
	SET( GLEW_FOUND FALSE )
	MESSAGE( STATUS "Looking for GLEW - not found" )
ENDIF ( GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES )