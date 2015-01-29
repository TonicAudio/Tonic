#include directories helps to include header files
include_directories(".")
include_directories(${INCLUDE_DIR})

#get the source filenames to compile
file(GLOB_RECURSE SOURCES Tonic/*.cpp)

add_definitions("${CFLAGS}")
link_libraries("${LINK_LIBS}")
link_libraries("${EXTRA_LIBS}")

#creating object file list from the sources ignoring main file
foreach(filename ${SOURCES})
	
	if("${filename}" MATCHES "main.c")

	else("${filename}" MATCHES "main.c")
		list(APPEND OBJECTS ${filename})
	endif("${filename}" MATCHES "main.c")

endforeach(filename ${SOURCES})

if(${PRJ_TYPE_EXECUTABLE})
	#create executable if the project type is PRJ_TYPE_EXECUTABLE
	add_executable(${PRJ_NAME} ${SOURCES})

	#always create libraries if PRJ_BUILD_LIBRARY defined
	if(${PRJ_BUILD_LIBRARY})
		add_library(${PRJ_NAME}-static STATIC ${OBJECTS})
		add_library(${PRJ_NAME}-shared SHARED ${OBJECTS})
	endif(${PRJ_BUILD_LIBRARY})

else(${PRJ_TYPE_EXECUTABLE})

	#creating libraries if not a executable project
	add_library(${PRJ_NAME}-static STATIC ${OBJECTS})
	add_library(${PRJ_NAME}-shared SHARED ${OBJECTS})
endif(${PRJ_TYPE_EXECUTABLE})

