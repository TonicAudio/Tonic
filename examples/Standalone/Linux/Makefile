# Tonic Standalone Demo Makefile
#
# Created by @anhinga
#
# Last updated 2013/07/02

##############################################################
#
# NOTE: Please set this to the Audio API you intend to use.
#
# Each API has its own set of linker dependencies, which are
# automatically added to the linker flags below.
#
# Valid values are:
#	LINUX_ALSA
#	LINUX_PULSE
#	LINUX_OSS
#	UNIX_JACK

AUDIO_API = LINUX_PULSE

###############################################################

# == Variables ==

CC = g++
#CCFLAGS = -m32

RTAUDIO_DIR = ../RTAudio
TONIC_SRC_DIR = ../../../src
TONIC_LIB_DIR = lib
INCLUDE_FLAGS = -I$(TONIC_SRC_DIR) -I$(TONIC_SRC_DIR)/Tonic

TONIC_CPP_FILES := $(wildcard $(TONIC_SRC_DIR)/Tonic/*.cpp)
TONIC_OBJ_FILES := $(addprefix $(TONIC_LIB_DIR)/linux/obj/,$(notdir $(TONIC_CPP_FILES:.cpp=.o)))


# libraries for RtAudio

ifeq ($(AUDIO_API),LINUX_ALSA)
RTAUDIO_LIBS = -lasound -lpthread
endif

ifeq ($(AUDIO_API),LINUX_OSS)
RTAUDIO_LIBS = -lpthread
endif

ifeq ($(AUDIO_API),LINUX_PULSE)
RTAUDIO_LIBS = -lpulse-simple -lpulse -lpthread
endif

ifeq ($(AUDIO_API),UNIX_JACK)
RTAUDIO_LIBS = `pkg-config --cflags --libs jack` -lpthread
endif

# == Targets ==

demo: libTonicLib.a
	$(CC) $(CCFLAGS) $(INCLUDE_FLAGS) -I$(RTAUDIO_DIR) -D__$(AUDIO_API)__ -L$(TONIC_LIB_DIR)/linux ../TonicStandaloneDemo/main.cpp $(RTAUDIO_DIR)/RtAudio.cpp -lTonicLib $(RTAUDIO_LIBS) -o demo 

libTonicLib.a: $(TONIC_OBJ_FILES)
	ar rc $(TONIC_LIB_DIR)/linux/libTonicLib.a $(TONIC_OBJ_FILES)

$(TONIC_LIB_DIR)/linux/obj/%.o: $(TONIC_SRC_DIR)/Tonic/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -c $(INCLUDE_FLAGS) -o $@ $<

clean:
	rm -rf $(TONIC_LIB_DIR) $(TONIC_OBJ_DIR) demo
