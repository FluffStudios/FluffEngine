SET(_PF86 "PROGRAMFILES(X86)")
SET( GLM_SEARCH_PATHS
	${GLM_ROOT_DIR}					# GLM!
	${FluffEngine_SOURCE_DIR}/Engine/Dependencies/glm
	$ENV{PROGRAMFILES}/GLM				# WINDOWS
	"$ENV{_PF86}/GLM"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( GLM_INCLUDE_DIRS
	NAMES
		glm/glm.hpp
	PATHS
		${GLM_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where glm.hpp resides"
)

FIND_LIBRARY( GLM_LIBRARIES
	NAMES
		glm_static
	PATHS
		${GLM_SEARCH_PATHS}
	PATH_SUFFIXES
		lib/${CMAKE_BUILD_TYPE}/x64
	DOC
		"The GLM library"
)

# Check if we found it!
IF ( GLM_INCLUDE_DIRS AND GLM_LIBRARIES )
	SET( GLM_FOUND TRUE )
	MESSAGE( STATUS "Looking for GLM - found" )
ELSE ( GLM_INCLUDE_DIRS AND GLM_LIBRARIES )
	SET( GLM_FOUND FALSE )
	MESSAGE( STATUS "Looking for GLM - not found" )
ENDIF ( GLM_INCLUDE_DIRS AND GLM_LIBRARIES )