#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    backgroundColor = ofColor(0,0,0);
    ofClear(backgroundColor);
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(true);
    ofSetWindowTitle("Listening Machines 1");
    
    //setup audio stream
    int numberOfOutputChannels = 0;
    int numberOfInputChannels = 2;
    int sampleRate = 44100;
    int frameSize = 1024;
    int numberOfBuffers = 1;
    
    soundStream.setup(numberOfOutputChannels, numberOfInputChannels, sampleRate, frameSize, numberOfBuffers);
    
    leftBuffer.assign(frameSize, 0);
    rightBuffer.assign(frameSize, 0);
    audioBuffer.assign(frameSize, 0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(backgroundColor);
    ofNoFill();
    
    int bucketWidth = (ofGetWidth() / audioBuffer.size());
    
    for (int i = 0; i < audioBuffer.size(); i++) {
        ofDrawRectangle(i * bucketWidth, ofGetHeight() / 2, bucketWidth, audioBuffer[i] * 1000);
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &inBuffer){
    int buffCounter = 0;
    
    for (int i = 0; i < inBuffer.getBuffer().size(); i+= 2) {
        leftBuffer[buffCounter] = inBuffer.getBuffer()[i];
        rightBuffer[buffCounter] = inBuffer.getBuffer()[i + 1];
        //Important: the buffer stores one sample from right and then the next one is from left channel, that's why using i and i+1
        audioBuffer[buffCounter] = (inBuffer.getBuffer()[i] + inBuffer.getBuffer()[i + 1]) / 2;
        buffCounter++;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
