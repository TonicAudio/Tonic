cmake_minimum_required(VERSION 2.6)

#set project name
set(PRJ_NAME Tonic)
#indicate type of the project as executable, otherwise it is a library
#set(PRJ_TYPE_EXECUTABLE 1)
#allways build libarary (with object files)
set(PRJ_BUILD_LIBRARY 1)
#main file name - used to filter this with object files
#set(PRJ_MAIN_FILE "main.c")

#libraries to link
set(LINK_LIBS "")
#define cflags
set(CFLAGS "")
#define header include directories as list
set(INCLUDE_DIR )

#set(WARNING_CFLAGS "-pedantic")
set(OTHER_CFLAGS "-pedantic")

# Including the required Frameworks
IF(APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   SET(EXTRA_LIBS 
	   	"-framework CoreFoundation" 
	   	"-framework CoreAudio" 
	   	"-framework Accelerate" 
	   	"-framework AudioToolbox")
   message("using frameworks ${EXTRA_LIBS}")
ENDIF (APPLE)

project(${PRJ_NAME})

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

#set enable testing
enable_testing()