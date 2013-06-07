#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofBackground(255);
    
    currentParameter = 0;
    
    // setup rtAudio
    const int bufferSize		= 512;
	const int sampleRate        = 44100;
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    setupSynth();
}

void testApp::setupSynth()
{
    
    // ------ Here we can instantiate any of the Demo Synths by name! -----
    
    // set this to the name of one of the Demo Synths
    string synthName = "DelayTestSynth";
    
    // if a synth with that name does not exist, error message will be logged
    synth = SynthFactory::createInstance(synthName);
    
    // Get some parameters - we will map mouse to these later
    synthParameters = synth.getParameters();
    
    // ---------------------------------------
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    if (synthParameters.size() > 0){
        
        // print some info about what's going on
        ofSetColor(0);
                
        ofDrawBitmapString("Click and drag up and down to modify parameter. Spacebar to cycle through parameters", 40, 60);
        
        ControlParameter param = synthParameters[currentParameter];
        string paramName = param.getDisplayName();
        float value = param.getValue();
        
        ofDrawBitmapString("Parameter " + ofToString(currentParameter+1) + " of " + ofToString(synthParameters.size()), 40, 100);
        
        ofDrawBitmapString(paramName + ": " + ofToString(value), 40, 120);
        
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        currentParameter = (currentParameter + 1) % synthParameters.size();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    if (synthParameters.size() > 0){
        ControlParameter currentParam = synthParameters[currentParameter];
        currentParam.setNormalizedValue(ofClamp(1.0f - (float)y/ofGetHeight(), 0.f, 1.f));
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
