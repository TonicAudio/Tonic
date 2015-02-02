## Testing the project

#include files including headers in the src
include_directories(. ../src)
include_directories(${INCLUDE_DIR})

#add test sources
file(GLOB TEST_SOURCES tonictests.cpp)

#set enable testing
enable_testing()
add_definitions("${CFLAGS}")
link_libraries("${LINK_LIBS}")
link_libraries("${PRJ_NAME}")
link_libraries("${EXTRA_LIBS}")

#adding tests foreach test source file
foreach(filename ${TEST_SOURCES})

	#get the basename of the source file
	get_filename_component(testname ${filename} NAME)

	#remove .cpp extension
	string(REGEX REPLACE ".cpp" "" testname "${testname}" )

	#create executable
	add_executable(${testname} ${SOURCES} ${filename})
	
	#configure for testing
	add_test(${testname} ${testname})

	message("using testsuite ${testname}")

endforeach(filename)