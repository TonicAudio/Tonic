## Testing the project

#include files including headers in the src
include_directories(. ./RTAudio ../../src )
include_directories(${INCLUDE_DIR})

#add sources
file(GLOB STANDALONE_SOURCES TonicStandaloneDemo/*.cpp RTAudio/*.cpp )

set(TONIC_USE_STANDALONE_EXAMPLE ON)

# include RTAudio Config directly
include(RTAudio/CMakeLists.txt)

# special case for MinGW

#set enable testing
enable_testing()
add_definitions(${CFLAGS})
link_libraries(${LINK_LIBS} ${RTAudio_LINKLIBS})
link_libraries(${PRJ_NAME})
link_libraries(${EXTRA_LIBS})


#create executable
add_executable(TonicStandaloneDemo ${OBJECTS} ${STANDALONE_SOURCES})

if (NOT TONIC_USE_STANDALONE_EXAMPLE)
  # disable the standalone target
  set_target_properties(TonicStandaloneDemo 
      PROPERTIES EXCLUDE_FROM_ALL 1 
      EXCLUDE_FROM_DEFAULT_BUILD 1
  )
endif(NOT TONIC_USE_STANDALONE_EXAMPLE)