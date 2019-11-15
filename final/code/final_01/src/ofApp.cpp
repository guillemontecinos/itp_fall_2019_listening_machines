#include "ofApp.h"

//Final_01
//Mesh animated with HPCP
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    
    // setup the audio stream
    soundStream.setup(numberOfOutputChannels, numberOfInputChannels, sampleRate, frameSize, numberOfBuffers);
    
    // mltk setup
    mltk.setup(frameSize, sampleRate, frameSize/2);
    mltk.run();
    
    // GUI
    gui.setup();
    gui.add(coefsNoveltyFactor.set("MFCC Coefs Novelty Factor", 1.0, 0.0001, 5000.0));
    gui.add(coefNumber.set("MFCC Coeficient", 4, 0, 12));
    gui.add(displayScope.setup("Display Scope"));
    
    // mfcc coeficients buffer setup
    numCoefs = 12;
    coefsBuffer.resize(numCoefs);
    coefsAvg.resize(numCoefs);
    for(int i = 0; i < coefsBuffer.size(); i++){
        coefsBuffer[i].resize(5);
        coefsAvg[i].resize(2);
    }
    
    base.set(0,0,0);
    vec1.set(100,0,0);
    vec2.set(100,0,0);
    vec3.set(100,0,0);
    
    baseMesh.setMode(OF_PRIMITIVE_LINES);
    
    //=============== Build Mesh ===============
//    for (float t = 0; t < 60; t += 0.01) {
    for (float t = 0; t < 20; t += 0.01) {
        vec1.scale(100 * ofNoise(t));
        vec2.scale(200 * ofNoise(2.71828 * t));
        vec3.scale(300 * ofNoise(5000 * t));
        qRot1.makeRotate(200 * t, ofVec3f(0,0,1));
        rotVec1 = qRot1 * vec1;
        qRot2.makeRotate(100 * 2.71828 * t, ofVec3f(0,1,0));
        rotVec2 = qRot2 * vec2;
        vecOut = rotVec1 + rotVec2;
        qRot3.makeRotate(200 * 2.71828 * t, ofVec3f(1,0,0));
        vecOut = vecOut + rotVec3;
        baseMesh.addVertex(vecOut);
        offsets.push_back(ofVec3f(ofNoise(t), ofNoise(t), ofNoise(t)));
    }
    
    //  connect mesh points
    int numVerts = baseMesh.getNumVertices();
    for (int a = 0; a < numVerts - 1; a++) {
        baseMesh.addIndex(a);
        baseMesh.addIndex(a + 1);
    }
    
    //  scale mesh radius
    for (int a = 0; a < numVerts; a++) {
        ofVec3f vert = baseMesh.getVertex(a);
        float len = vert.length();
        vert.scale(len/300);
        baseMesh.setVertex(a, vert);
    }
    
    drawMesh = baseMesh;
    //=============== Build Mesh ===============
}

//--------------------------------------------------------------
void ofApp::update(){
    // This will show us the framerate in the window's title bar
    if(showFps.get()){
        ofSetWindowTitle("Listening Machine - Week 2 - " + to_string(ofGetFrameRate()));
    }
    
    // this runs the analysis chain that's been declared inside ofxMLTK
    mltk.run();
    
    vector<Real> HPCP = mltk.getData("HPCP");
//    cout << HPCP << endl;
    vector<Real> mfcc_coefs = mltk.getData("MFCC.coefs");
    for (int i = 0; i < coefsBuffer.size(); i++) {
        float avg = 0;
        for (int j = 0; j < coefsBuffer[i].size(); j++) {
            if (j < coefsBuffer[i].size() - 1) {
                coefsBuffer[i][j] = (float)coefsBuffer[i][j + 1];
            }
            else{
                coefsBuffer[i][j] = ofMap(abs((float)HPCP[i]), 0.0f, 1.0f, 0.0f, coefsNoveltyFactor.get());
            }
            avg += coefsBuffer[i][j];
        }
        coefsAvg[i][0] = coefsAvg[i][1];
        coefsAvg[i][1] = avg / (float)coefsBuffer[i].size();
        cout << coefsAvg << endl;
    }
    

    //=============== Update Mesh ===============
    int numVerts = drawMesh.getNumVertices();
    int range = (int)numVerts / (HPCP.size() - 1); //we need to interpolate with the coefs in the borders
    //"i = range + 1" to get rid of the first coeeficient which is extreamly high
    for (int i = 0; i < numVerts - 1; ++i) {
        //TODO: estimate scale according the interpolation between one vertex and the following. Use ofInterpolateCosine. Create a variable rangeLength to store how meny vertex fit between two coeffs
        
        int coefCount = (int)(i / range);
        float step = (float)(i % range) / (float)range;
        
        if(coefCount < coefsAvg.size() - 1){
            scale = ofLerp(coefsAvg[coefCount][1], coefsAvg[coefCount + 1][1], step);
        }
        else{
            scale = coefsAvg[coefCount][1];
        }
        
        ofVec3f vert = baseMesh.getVertex(i);
        float time = ofGetElapsedTimef();
        float timeScale = 0.5;
        ofVec3f timeOffsets = offsets[i];
        float len = vert.length();
        vert.scale(len * (1 + ofMap(ofSignedNoise(time * timeScale + timeOffsets.length()),-1,1,-0.2,0.2)) + scale);
        drawMesh.setVertex(i, vert);
    }
    //=============== Update Mesh ===============
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(50);
    if(displayScope){
        float mfccCoefsWidth = (ofGetWidth() / float(coefsAvg.size()));

        ofSetColor(0);

        ofPolyline line;

        ofSetLineWidth(6);
        for(int i = 0; i < coefsAvg.size(); i++){
            line.lineTo(i * mfccCoefsWidth,
                        ofGetHeight()/2 + ((coefsAvg[i][1])*coefsNoveltyFactor.get()));
        }
        line.lineTo(ofGetWidth(), ofGetHeight()/2);
        if(showCoefs){
            line.draw();
        }
    }
    ofSetColor(255);
    ofSetLineWidth(2);
    cam.begin();
        drawMesh.draw();
    cam.end();
    
    if(showGui){
        gui.draw();
    }
}


//-----
void ofApp::audioIn(ofSoundBuffer &inBuffer){
    inBuffer.getChannel(mltk.leftAudioBuffer, 0);
    inBuffer.getChannel(mltk.rightAudioBuffer, 1);
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
