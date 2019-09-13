#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    backgroundColor = ofColor(0,0,0);
    ofClear(backgroundColor);
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(true);
    ofSetWindowTitle("Listening Machines 1");
    
    //setup audio stream
    int numberOfOutputChannels = 2;
    int numberOfInputChannels = 2;
    int numberOfBuffers = 1;
    sampleRate = 44100;
    frameSize = 1024;
    
    soundStream.setup(numberOfOutputChannels, numberOfInputChannels, sampleRate, frameSize, numberOfBuffers);
    
    leftBuffer.assign(frameSize, 0);
    rightBuffer.assign(frameSize, 0);
    audioBuffer.assign(frameSize, 0);
    audioBuffer150.assign(sampleRate * 0.15, 0);
    audioBuffer300.assign(sampleRate * 0.3, 0);
    audioBuffer500.assign(sampleRate * 0.5, 0); // sampleRate = 44,100 samples/second * 500 milliseconds = 22,050 samples
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
//  shift audiobuffer300
    for (int i = 0; i < audioBuffer150.size(); i++) {
        if (i < audioBuffer150.size() - frameSize) {
            audioBuffer150[i] = audioBuffer150[i + frameSize];
        }
        else {
            audioBuffer150[i] = audioBuffer[i - audioBuffer150.size() + frameSize];
        }
    }
//  shift audiobuffer300
    for (int i = 0; i < audioBuffer300.size(); i++) {
        if (i < audioBuffer300.size() - frameSize) {
            audioBuffer300[i] = audioBuffer300[i + frameSize];
        }
        else {
            audioBuffer300[i] = audioBuffer[i - audioBuffer300.size() + frameSize];
        }
    }
//  shift audiobuffer500
    for (int i = 0; i < audioBuffer500.size(); i++) {
        if (i < audioBuffer500.size() - frameSize) {
            audioBuffer500[i] = audioBuffer500[i + frameSize];
        }
        else {
            audioBuffer500[i] = audioBuffer[i - audioBuffer500.size() + frameSize];
        }
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer){
//========================
    float decay150 = 0.6f;
    float decay300 = 0.4f;
    float decay500 = 0.2f;
    for (int i = 0; i < audioBuffer.size(); i++) {
        audioBuffer[i] += (double)((float)audioBuffer150[i] * decay150 + (float)audioBuffer300[i] * decay300 + (float)audioBuffer500[i] * decay500) * .3;
    }
    
    
    
//========================

//    Mono output
    int buffCounter = 0;
    for(int i = 0; i < audioBuffer.size(); i++) {
        outBuffer[buffCounter] = audioBuffer[i]; // writing to the left channel
        outBuffer[buffCounter + 1] = audioBuffer[i];
        buffCounter += 2;
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
