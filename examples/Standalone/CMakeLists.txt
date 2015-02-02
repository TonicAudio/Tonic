## Testing the project

#include files including headers in the src
include_directories(. ./RTAudio ../../src )
include_directories(${INCLUDE_DIR})

#add sources
file(GLOB STANDALONE_SOURCES TonicStandaloneDemo/*.cpp RTAudio/*.cpp )

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

# MinGW support for dsound requires locating that libraries, Visual Studio
# does not require that step
if (AUDIO_WINDOWS_DS)
	if ("${CMAKE_GENERATOR}" STREQUAL "MinGW Makefiles")
      # looking for directX
      include("RtAudio/FindDirectX.cmake")
	    
	    if (NOT DirectX_FOUND)
	    
	        message( STATUS "Directsound was not found" )
	        message( STATUS "Standalone example can't be compiled" )

	        # disable the standalone target
	        set_target_properties(TonicStandaloneDemo 
	        	PROPERTIES EXCLUDE_FROM_ALL 1 
	        	EXCLUDE_FROM_DEFAULT_BUILD 1
	      	)
    	
    	else(NOT DirectX_FOUND)
      
        target_link_libraries(DirectX_LIBRARY)
        target_compile_definitions("-I${DirectX_INCLUDE_DIR}" )
	    
	    endif(NOT DirectX_FOUND)

	endif("${CMAKE_GENERATOR}" STREQUAL "MinGW Makefiles")
endif(AUDIO_WINDOWS_DS)