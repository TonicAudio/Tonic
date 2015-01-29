## Testing the project

#include files including headers in the src
include_directories(. ../src)
include_directories(${INCLUDE_DIR})

#add test sources
file(GLOB TEST_SOURCES *.cpp)

#set enable testing
enable_testing()
add_definitions("${CFLAGS}")
link_libraries("${LINK_LIBS}")
link_libraries("${PRJ_NAME}")
link_libraries("${EXTRA_LIBS}")

#adding tests foreach test source file
foreach(filename ${TEST_SOURCES})
	
	#get the file name of the source file
	if(WIN32) 
		string(REGEX REPLACE ".*\\\\" "" testname "${filename}" )
	else(WIN32)
		string(REGEX REPLACE ".*/" "" testname "${filename}" )
	endif(WIN32)

	#remove .cpp extension
	string(REGEX REPLACE ".cpp" "" testname "${testname}" )

	#create executable
	add_executable(${testname} ${SOURCES} ${filename})
	#configure for testing
	add_test(${testname} ${testname})

	message("${testname} ${filename}")
endforeach(filename)