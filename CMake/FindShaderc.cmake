SET(_PF86 "PROGRAMFILES(X86)")
SET( SHADERC_SEARCH_PATHS
	${SHADERC_ROOT_DIR}					# SHADERC!
	./Engine/Dependencies/shaderc
	$ENV{PROGRAMFILES}/Shaderc				# WINDOWS
	"$ENV{_PF86}/Shaderc"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( SHADERC_INCLUDE_DIRS
	NAMES
		shaderc/shaderc.h
	PATHS
		${SHADERC_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where shaderc.h resides"
)

FIND_LIBRARY( SHADERC_LIBRARIES
	NAMES
        shaderc_combined
	PATHS
		${SHADERC_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/Release/Win32
		lib/Release/x64
	DOC
		"The Shaderc library"
)

# Check if we found it!
IF ( SHADERC_INCLUDE_DIRS AND SHADERC_LIBRARIES )
	SET( SHADERC_FOUND TRUE )
	MESSAGE( STATUS "Looking for Shaderc - found" )
ELSE ( SHADERC_INCLUDE_DIRS AND SHADERC_LIBRARIES )
	SET( SHADERC_FOUND FALSE )
	MESSAGE( STATUS "Looking for Shaderc - not found" )
ENDIF ( SHADERC_INCLUDE_DIRS AND SHADERC_LIBRARIES )