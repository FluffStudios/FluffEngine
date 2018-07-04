SET(_PF86 "PROGRAMFILES(X86)")
SET( PHYSX_SEARCH_PATHS
	${PHYSX_ROOT_DIR}					# PHYSX!
	./Engine/Dependencies/physx
	$ENV{PROGRAMFILES}/Physx				# WINDOWS
	"$ENV{_PF86}/Physx"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( PHYSX_INCLUDE_DIRS
	NAMES
        Physx/PxPhysics.h
	PATHS
		${PHYSX_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where PxPhysics.h resides"
)

FIND_LIBRARY( PHYSX_LIBRARIES
	NAMES
        physx
	PATHS
		${PHYSX_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/Release/Win32
		lib/Release/x64
	DOC
		"The Physx library"
)

# Check if we found it!
IF ( PHYSX_INCLUDE_DIRS AND PHYSX_LIBRARIES )
	SET( PHYSX_FOUND TRUE )
	MESSAGE( STATUS "Looking for Physx - found" )
ELSE ( PHYSX_INCLUDE_DIRS AND PHYSX_LIBRARIES )
	SET( PHYSX_FOUND FALSE )
	MESSAGE( STATUS "Looking for Physx - not found" )
ENDIF ( PHYSX_INCLUDE_DIRS AND PHYSX_LIBRARIES )