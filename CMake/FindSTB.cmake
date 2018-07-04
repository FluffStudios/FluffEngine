SET(_PF86 "PROGRAMFILES(X86)")
SET( STB_SEARCH_PATHS
	${STB_ROOT_DIR}					# STB!
	./Engine/Dependencies/stb
	$ENV{PROGRAMFILES}/STB				# WINDOWS
	"$ENV{_PF86}/STB"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

FIND_PATH( STB_INCLUDE_DIRS
	NAMES
        stb_image.h
        stb_image_write.h
	PATHS
		${STB_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where stb_image.h and stb_image_write.h resides"
)

# Check if we found it!
IF ( STB_INCLUDE_DIRS )
	SET( STB_FOUND TRUE )
	MESSAGE( STATUS "Looking for STB - found" )
ELSE ( STB_INCLUDE_DIRS )
	SET( STB_FOUND FALSE )
	MESSAGE( STATUS "Looking for STB - not found" )
ENDIF ( STB_INCLUDE_DIRS )