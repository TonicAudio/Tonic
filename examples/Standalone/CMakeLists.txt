## Testing the project

#include files including headers in the src
include_directories(. ./RTAudio ../../src )
include_directories(${INCLUDE_DIR})

#add sources
file(GLOB STANDALONE_SOURCES TonicStandaloneDemo/*.cpp RTAudio/*.cpp )

#set enable testing
enable_testing()
add_definitions("${CFLAGS}")
link_libraries("${LINK_LIBS}")
link_libraries("${PRJ_NAME}")
link_libraries("${EXTRA_LIBS}")

# include RTAudio Config directly
include(RTAudio/CMakeLists.txt)

#create executable
add_executable(TonicStandaloneDemo ${OBJECTS} ${STANDALONE_SOURCES})