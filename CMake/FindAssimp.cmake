SET(_PF86 "PROGRAMFILES(X86)")
SET( ASSIMP_SEARCH_PATHS
	${ASSIMP_ROOT_DIR}					# ASSIMP!
	${FluffEngine_SOURCE_DIR}/Engine/Dependencies/assimp
	$ENV{PROGRAMFILES}/Assimp				# WINDOWS
	"$ENV{_PF86}/Assimp"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( ASSIMP_INCLUDE_DIRS
	NAMES
        assimp/anim.h
        assimp/ai_assert.h
	PATHS
		${ASSIMP_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where anim.h and ai_assert.h resides"
)

set(ASSIMP_LIBRARIES "${FluffEngine_DEPEND_DIR}/assimp/lib/${_config}/x64/assimp.lib")

# Check if we found it!
IF ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
	SET( ASSIMP_FOUND TRUE )
	MESSAGE( STATUS "Looking for ASSIMP - found" )
ELSE ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
	SET( ASSIMP_FOUND FALSE )
	MESSAGE( STATUS "Looking for ASSIMP - not found" )
ENDIF ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )