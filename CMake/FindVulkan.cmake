SET(_PF86 "PROGRAMFILES(X86)")
SET( VULKAN_SEARCH_PATHS
	${VULKAN_ROOT_DIR}					# VULKAN!
	./Engine/Dependencies/vulkan
	$ENV{PROGRAMFILES}/Vulkan				# WINDOWS
	"$ENV{_PF86}/Vulkan"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( VULKAN_INCLUDE_DIRS
	NAMES
		vulkan/vulkan.h
	PATHS
		${VULKAN_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where vulkan.h resides"
)

FIND_LIBRARY( VULKAN_LIBRARIES
	NAMES
        vulkan-1 VKstatic.1
	PATHS
		${VULKAN_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/Release/Win32
		lib/Release/x64
	DOC
		"The Vulkan library"
)

# Check if we found it!
IF ( VULKAN_INCLUDE_DIRS AND VULKAN_LIBRARIES )
	SET( VULKAN_FOUND TRUE )
	MESSAGE( STATUS "Looking for Vulkan - found" )
ELSE ( VULKAN_INCLUDE_DIRS AND VULKAN_LIBRARIES )
	SET( VULKAN_FOUND FALSE )
	MESSAGE( STATUS "Looking for Vulkan - not found" )
ENDIF ( VULKAN_INCLUDE_DIRS AND VULKAN_LIBRARIES )