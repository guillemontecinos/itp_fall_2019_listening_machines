#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMLTK.h"

class ofApp : public ofBaseApp {
public:
    bool showGui = true;
    bool showBands = true;
    bool showCoefs = true;
    
    int numberOfOutputChannels = 0;
    int numberOfInputChannels = 2;
    int sampleRate = 44100;
    int frameSize = 1024;
    int numberOfBuffers = 4;
    
    ofMutex mutex;
    MLTK mltk;
    
    ofxPanel gui;
    
    ofParameter<bool> showFps;
    ofParameter<float> coefsNoveltyFactor;
    ofParameter<int> coefNumber;
    ofxToggle displayScope;
    
    vector<vector<float>> coefsBuffer;
    vector<vector<float>> coefsAvg;
    int numCoefs;
    
    ofSoundStream soundStream;
    
    void audioIn(ofSoundBuffer &inBuffer);
    
    // These are the built-ins
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofEasyCam cam;
    
    //=============== Mesh Stuff ===============
    ofVec3f base, vec1, vec2, vec3, vecOut, rotVec1, rotVec2, rotVec3;
    ofQuaternion qRot1, qRot2, qRot3;
    ofMesh baseMesh, drawMesh;
    vector<ofVec3f> offsets;
    float scale;
    //=============== Mesh Stuff ===============
    
};
