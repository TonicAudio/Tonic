# RtAudio: a set of realtime audio i/o C++ classes
# Copyright (c) 2001-2014 Gary P. Scavone

# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# Any person wishing to distribute modifications to the Software is
# asked to send the modifications to the original developer so that
# they can be incorporated into the canonical version.  This is,
# however, not a binding provision of this license.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
# ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# cmake_minimum_required(VERSION 2.8.10)
# project(RtAudio)

# include(CTest)
# include(CheckFunctionExists)

option(BUILD_PYTHON "Build PyRtAudio python bindings" OFF)
option(AUDIO_WINDOWS_DS "Build Windows DirectSound API" OFF)

# TODO: ASIO currently not supported in Tonic
#option(AUDIO_WINDOWS_ASIO "Build Windows ASIO API" OFF)
set(AUDIO_WINDOWS_ASIO OFF)

# TODO: WSAPI is currently not supported in Tonic
#option(AUDIO_WINDOWS_WASAPI "Build Windows WASAPI API" OFF)
set(AUDIO_WINDOWS_WASAPI OFF)

option(AUDIO_LINUX_OSS "Build Linux OSS API" OFF)
option(AUDIO_LINUX_ALSA "Build Linux ALSA API" OFF)
option(AUDIO_LINUX_PULSE "Build Linux PulseAudio API" OFF)
option(AUDIO_UNIX_JACK "Build Unix JACK audio server API" OFF)
option(AUDIO_OSX_CORE "Build Mac OSX CoreAudio API" OFF)

# for development currently
option(AUDIO_FORCE_MINGW "force MinGW passing (for development)" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D__RTAUDIO_DEBUG__)
endif ()

#check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)

#if (HAVE_GETTIMEOFDAY)
#    add_definitions(-DHAVE_GETTIMEOFDAY)
#endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif (CMAKE_COMPILER_IS_GNUCXX)

set(rtaudio_SOURCES RtAudio.cpp)

set(RTAudio_LINKLIBS)
if (CMAKE_SYSTEM_NAME MATCHES "kNetBSD.*|NetBSD.*")
    message(STATUS "NetBSD detected, using OSS")
    find_package(Threads REQUIRED CMAKE_THREAD_PREFER_PTHREAD)
    list(APPEND RTAudio_LINKLIBS ossaudio ${CMAKE_THREAD_LIBS_INIT})
    set(AUDIO_LINUX_OSS ON)
elseif (UNIX AND NOT APPLE)
    if (NOT AUDIO_LINUX_PULSE AND NOT AUDIO_LINUX_ALSA AND NOT AUDIO_LINUX_OSS AND NOT AUDIO_UNIX_JACK)
        set(AUDIO_LINUX_ALSA ON)
    endif()

    if (AUDIO_LINUX_PULSE)
        find_library(PULSE_LIB pulse)
        find_library(PULSESIMPLE_LIB pulse-simple)
        list(APPEND RTAudio_LINKLIBS ${PULSE_LIB} ${PULSESIMPLE_LIB})
        add_definitions(-D__LINUX_PULSE__)
        message(STATUS "Using Linux PulseAudio")
    endif (AUDIO_LINUX_PULSE)
    if (AUDIO_LINUX_ALSA)
        find_package(ALSA)
        find_package(Threads REQUIRED CMAKE_THREAD_PREFER_PTHREAD)
        if (NOT ALSA_FOUND)
            message(FATAL_ERROR "ALSA API requested but no ALSA dev libraries found")
        endif()
        list(APPEND RTAudio_LINKLIBS ${ALSA_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
        add_definitions(-D__LINUX_ALSA__)
        message(STATUS "Using Linux ALSA")
    endif (AUDIO_LINUX_ALSA)
endif ()

if (APPLE)
    if (NOT AUDIO_OSX_CORE AND NOT AUDIO_UNIX_JACK)
        set(AUDIO_OSX_CORE ON)
    endif()

    if (AUDIO_OSX_CORE)
        find_library(COREAUDIO_LIB CoreAudio)
        find_library(COREFOUNDATION_LIB CoreFoundation)
        list(APPEND RTAudio_LINKLIBS ${COREAUDIO_LIB} ${COREFOUNDATION_LIB})
        add_definitions(-D__MACOSX_CORE__)
        message(STATUS "Using OSX CoreAudio")
    endif (AUDIO_OSX_CORE)
endif (APPLE)

# JACK supported on many Unices
if (UNIX)
    if (AUDIO_UNIX_JACK)
        find_library(JACK_LIB jack)
        list(APPEND RTAudio_LINKLIBS ${JACK_LIB})
        add_definitions(-D__UNIX_JACK__)
        message(STATUS "Using JACK")
    endif (AUDIO_UNIX_JACK)
endif (UNIX)

if (WIN32)
    if (NOT AUDIO_WINDOWS_DS AND NOT AUDIO_WINDOWS_ASIO AND NOT AUDIO_WINDOWS_WASAPI)
        set(AUDIO_WINDOWS_DS ON)
    endif()

    include_directories(include)
    list(APPEND RTAudio_LINKLIBS winmm ole32)

    if (AUDIO_WINDOWS_DS)
        add_definitions(-D__WINDOWS_DS__)
        message(STATUS "Using Windows DirectSound")
        list(APPEND RTAudio_LINKLIBS dsound)
    endif (AUDIO_WINDOWS_DS)

# TODO: currently not supported in Tonic
#    if (AUDIO_WINDOWS_WASAPI)
#        add_definitions(-D__WINDOWS_WASAPI__)
#        message(STATUS "Using Windows WASAPI")
#        list(APPEND RTAudio_LINKLIBS uuid ksuser)
#        message( FATAL_ERROR "using WSAPI is currently not supported in Tonic" )
#    endif (AUDIO_WINDOWS_WASAPI)


# TODO: currently not supported in Tonic
#    if (AUDIO_WINDOWS_ASIO)
#        list(APPEND rtaudio_SOURCES
#            include/asio.cpp
#            include/asiodrivers.cpp
#            include/asiolist.cpp
#            include/iasiothiscallresolver.cpp)
#        add_definitions(-D__WINDOWS_ASIO__)
#        message(STATUS "Using Windows ASIO")
#    endif (AUDIO_WINDOWS_ASIO)
endif (WIN32)

# add_library(rtaudio SHARED ${rtaudio_SOURCES})
# add_library(rtaudio_static STATIC ${rtaudio_SOURCES})

# target_link_libraries(rtaudio ${RTAudio_LINKLIBS})

# if (BUILD_TESTING)
#    add_subdirectory(tests)
# endif (BUILD_TESTING)

# MinGW support for dsound requires locating that libraries, Visual Studio
# does not require that step
if (AUDIO_WINDOWS_DS)
    if ("${CMAKE_GENERATOR}" STREQUAL "MinGW Makefiles")
      # looking for directX
      include("RTAudio/FindDirectX.cmake")
        
        if (NOT DirectX_FOUND)
        
            message( STATUS "Directsound was not found" )
            message( STATUS "Standalone example can't be compiled" )

            set(TONIC_USE_STANDALONE_EXAMPLE OFF)
        
        else(NOT DirectX_FOUND)
      
        target_link_libraries(DirectX_LIBRARY)
        target_compile_definitions("-I${DirectX_INCLUDE_DIR}" )
        
        endif(NOT DirectX_FOUND)

    endif("${CMAKE_GENERATOR}" STREQUAL "MinGW Makefiles")
endif(AUDIO_WINDOWS_DS)
