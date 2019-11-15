#pragma once

#include "ofxiOS.h"
#include <ARKit/ARKit.h>
#include "ofxARKit.h"
#include "ofxGui.h"
#include "ofxMLTK.h"
class ofApp : public ofxiOSApp {
    
public:
    
    ofApp (ARSession * session);
    ofApp();
    ~ofApp ();
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void touchDown(ofTouchEventArgs &touch);
    void touchMoved(ofTouchEventArgs &touch);
    void touchUp(ofTouchEventArgs &touch);
    void touchDoubleTap(ofTouchEventArgs &touch);
    void touchCancelled(ofTouchEventArgs &touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    ofTrueTypeFont font;
    ofImage img;
    bool noMesh;

    // ====== AR STUFF ======== //
    vector < matrix_float4x4 > mats;
    vector<ARAnchor*> anchors;
    ofCamera camera;
    ARSession * session;
    ARRef processor;
    
    //=============== Sound Stuff ===============
    ofMutex mutex;
    MLTK mltk;
    ofSoundStream soundStream;
    void audioIn(ofSoundBuffer &inBuffer);
//    void audioIn(float *input, int bufferSize, int nChannels);
    void audioOut(ofSoundBuffer &outBuffer);
//    void audioOut(float *output, int bufferSize, int nChannels);
    ofSoundBuffer patchBuffer;
    
    bool showGui = true;
    bool showBands = true;
    bool showCoefs = true;
    
    int numberOfOutputChannels = 1;
    int numberOfInputChannels = 1;
    int sampleRate = 44100;
    int frameSize = 1024;
    int numberOfBuffers = 1;
    vector<vector<float>> coefsBuffer;
    vector<vector<float>> coefsAvg;
    int numCoefs;
    
    //=============== GUI Stuff ===============
    ofxPanel gui;
    ofParameter<bool> showFps;
    ofParameter<float> coefsNoveltyFactor;
    
    //=============== Mesh Stuff ===============
    ofVec3f base, vec1, vec2, vec3, vecOut, rotVec1, rotVec2, rotVec3;
    ofQuaternion qRot1, qRot2, qRot3;
    ofMesh baseMesh, drawMesh;
    vector<ofVec3f> offsets;
    float scale;
    //=============== Mesh Stuff ===============
};


