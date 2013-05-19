//
//  main.cpp
//  TonicStandaloneDemo
//
//  Created by Nick Donaldson on 5/16/13.
//
//

// This is a super-simple demo showing a very basic command-line C++ program to play a Tonic synth

#include <iostream>
#include "Tonic.h"
#include "RtAudio.h"

using namespace Tonic;

const unsigned int nChannels = 2;

Synth * synth = NULL;

int renderCallback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData )
{
    if (synth != NULL){
        synth->fillBufferOfFloats((float*)outputBuffer, nBufferFrames, nChannels);
    }
    
    return 0;
}

int main(int argc, const char * argv[])
{
    // Configure RtAudio
    RtAudio dac;
    RtAudio::StreamParameters rtParams;
    rtParams.deviceId = dac.getDefaultOutputDevice();
    rtParams.nChannels = nChannels;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256; // 256 sample frames
    
    // You don't necessarily have to do this - it will default to 44100 if not set.
    Tonic::setSampleRate(sampleRate);
    
    // --------- MAKE A SYNTH HERE -----------
    
    synth = new Synth();
    
    ControlGenerator freq = ControlValue(300);
    Generator freqSmoothed = freq.smoothed();
    
    Generator output = RectWave().freq(freq);
    
    synth->setOutputGen(output);
    
    // ---------------------------------------
    
    
    // open rtaudio stream
    try {
        dac.openStream( &rtParams, NULL, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, &renderCallback, NULL, NULL );
        
        dac.startStream();
        
        // hacky, yes, but let's just hang out for awhile until someone presses a key
        printf("\n\nPress any key to stop\n\n");
        cin.get();
        
        dac.stopStream();
    }
    catch ( RtError& e ) {
        std::cout << '\n' << e.getMessage() << '\n' << std::endl;
        exit( 0 );
    }
    
    return 0;
}

