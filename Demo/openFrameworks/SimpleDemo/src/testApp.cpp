#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    // setup rtAudio
    const int bufferSize		= 512;
	const int sampleRate        = 44100;
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    setupSynth();
}

void testApp::setupSynth()
{    
    // ------ Configure your synth here! ------
    
    // Paraemter for pitch as note number
    ControlParameter pitch = synth.addParameter("pitch", 60.f);
    
    // Parameter for triggering lfo
    ControlParameter lfoTrig = synth.addParameter("lfoTrig", 0);
    
    // Parameter for lfo frequency
    ControlParameter lfoFreq = synth.addParameter("lfoFreq", 5.0f);
    
    // Convert note number to Hz and smooth it
    Generator smoothFreq = (ControlMidiToFreq().input(pitch)).smoothed();
    
    // Envelope for lfo
    ADSR lfoEnv = ADSR(0.5f, 0.f, 1.f, 2.5f).trigger(lfoTrig).exponential(true);
    
    // lfo generator - multiply freq by this for 10% vibrato in either direction
    Generator lfo = 1.0f + (SineWave().freq(lfoFreq) * lfoEnv * 0.1f);
    
    // 2 sine waves an octave apart, with lfo
    Generator singingSines = (
    
        SineWave().freq( smoothFreq * lfo ) + (SineWave().freq( (smoothFreq * 2.f) * lfo ) * 0.5)
    
    ) * 0.25f;
    
    synth.setOutputGen(singingSines);
    
    // ---------------------------------------
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
    // normalize x, map to pitch
    synth.setParameter("pitch", ofMap((float)x/ofGetWidth(), 0.f, 1.f, 40.f, 80.f, true));
    
    // normalize y, map to lfo freq
    synth.setParameter("lfoFreq", ofMap((float)y/ofGetHeight(), 0.f, 1.f, 8.0f, 0.5f, true));
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    // treat like a move, just call mousemoved
    mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // mouse down press turns on lfo
    synth.setParameter("lfoTrig", 1.f);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    // mouse up press turns off lfo
    synth.setParameter("lfoTrig", 0.f);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

// give us a Tonic buffer!
void testApp::audioOut(float * input, int bufferSize, int nChannels)
{
    synth.fillBufferOfFloats(input, bufferSize, nChannels);
}
