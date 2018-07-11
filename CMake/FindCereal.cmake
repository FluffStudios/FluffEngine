SET(_PF86 "PROGRAMFILES(X86)")
SET( CEREAL_SEARCH_PATHS
	${CEREAL_ROOT_DIR}					# CEREAL!
	${FluffEngine_SOURCE_DIR}/Engine/Dependencies/cereal
	$ENV{PROGRAMFILES}/Cereal				# WINDOWS
	"$ENV{_PF86}/Cereal"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( CEREAL_INCLUDE_DIRS
	NAMES
		cereal/cereal.hpp
	PATHS
		${CEREAL_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where cereal.hpp resides"
)

# Check if we found it!
IF ( CEREAL_INCLUDE_DIRS )
	SET( CEREAL_FOUND TRUE )
	MESSAGE( STATUS "Looking for CEREAL - found" )
ELSE ( CEREAL_INCLUDE_DIRS )
	SET( CEREAL_FOUND FALSE )
	MESSAGE( STATUS "Looking for CEREAL - not found" )
ENDIF ( CEREAL_INCLUDE_DIRS )