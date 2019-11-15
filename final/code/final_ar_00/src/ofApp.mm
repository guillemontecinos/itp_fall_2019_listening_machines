#include "ofApp.h"

using namespace ofxARKit::common;

void logSIMD(const simd::float4x4 &matrix)
{
    std::stringstream output;
    int columnCount = sizeof(matrix.columns) / sizeof(matrix.columns[0]);
    for (int column = 0; column < columnCount; column++) {
        int rowCount = sizeof(matrix.columns[column]) / sizeof(matrix.columns[column][0]);
        for (int row = 0; row < rowCount; row++) {
            output << std::setfill(' ') << std::setw(9) << matrix.columns[column][row];
            output << ' ';
        }
        output << std::endl;
    }
    output << std::endl;
}

//--------------------------------------------------------------
ofApp :: ofApp (ARSession * session){
    this->session = session;
    cout << "creating ofApp" << endl;
}

ofApp::ofApp(){}

//--------------------------------------------------------------
ofApp :: ~ofApp () {
    cout << "destroying ofApp" << endl;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(127);
    
    img.load("OpenFrameworks.png");
    
    int fontSize = 8;
    if (ofxiOSGetOFWindow()->isRetinaSupportedOnDevice())
        fontSize *= 2;
    
    font.load("fonts/mono0755.ttf", fontSize);
    
    processor = ARProcessor::create(session);
    processor->setup();
    
    noMesh = true;
    
    glLineWidth(4);
    
    
    //=============== Sound Stuff ===============
    // setup the audio stream
    soundStream.setup(numberOfOutputChannels, numberOfInputChannels, sampleRate, frameSize, numberOfBuffers);
//    ofSoundStreamSetup(numberOfOutputChannels, numberOfInputChannels, sampleRate, frameSize, numberOfBuffers);
//    patchBuffer.resize(frameSize);
    patchBuffer.setSampleRate(sampleRate);
    // mltk setup
    mltk.setup(frameSize, sampleRate, frameSize/2);
    mltk.run();
    
    // mfcc coeficients buffer setup
    numCoefs = 13;
    coefsBuffer.resize(numCoefs);
    coefsAvg.resize(numCoefs);
    for(int i = 0; i < coefsBuffer.size(); i++){
        coefsBuffer[i].resize(5);
        coefsAvg[i].resize(2);
    }
    //=============== Sound Stuff ===============
    
    //=============== GUI Stuff ===============
    gui.setup();
    gui.add(coefsNoveltyFactor.set("MFCC Coefs Novelty Factor", 2.0, 0.0001, 10.0));
    //=============== GUI Stuff ===============
    
    //=============== Mesh Stuff ===============
    //all values here are proportianally ported from the local mesh version divided by 300
    base.set(0,0,0);
    vec1.set(0.33,0,0); //(100,0,0)
    vec2.set(0.33,0,0); //(100,0,0)
    vec3.set(0.33,0,0); //(100,0,0)
    
    baseMesh.setMode(OF_PRIMITIVE_LINES);
    //=============== Mesh Stuff ===============
    //=============== Build Mesh ===============
    
    for (float t = 0; t < 15; t += 0.01) {
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
}



//--------------------------------------------------------------
void ofApp::update(){
    
    processor->update();
    
    //=============== Sound Stuff ===============
    // this runs the analysis chain that's been declared inside ofxMLTK
    mltk.run();
    
    vector<Real> mfcc_coefs = mltk.getData("MFCC.coefs");
    for (int i = 0; i < coefsBuffer.size(); i++) {
        float avg = 0;
        for (int j = 0; j < coefsBuffer[i].size(); j++) {
            if (j < coefsBuffer[i].size() - 1) {
                coefsBuffer[i][j] = (float)coefsBuffer[i][j + 1];
            }
            else{
                //This is just to bring coef[0] to the range of the other coefs
                if (i == 0) {
                    coefsBuffer[i][j] = ofMap(abs((float)mfcc_coefs[i] / 100), 0.0f, 800.0f, 0.0f, coefsNoveltyFactor.get());
//                    coefsBuffer[i][j] = ofMap(abs((float)mfcc_coefs[i] / 100), 0.0f, 800.0f, 0.0f, 2.0f);
                }
                else{
                    coefsBuffer[i][j] = ofMap(abs((float)mfcc_coefs[i]), 0.0f, 800.0f, 0.0f, coefsNoveltyFactor.get());
//                    coefsBuffer[i][j] = ofMap(abs((float)mfcc_coefs[i]), 0.0f, 800.0f, 0.0f, 2.0f);
                }
            }
            avg += coefsBuffer[i][j];
        }
        coefsAvg[i][0] = coefsAvg[i][1];
        coefsAvg[i][1] = avg / (float)coefsBuffer[i].size();
    }
    //=============== Sound Stuff ===============
    
    //=============== Update Mesh ===============
    int numVerts = drawMesh.getNumVertices();
    int range = (int)numVerts / (numCoefs - 1);
    for (int i = 0; i < numVerts - 1; ++i) {
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
void ofApp::draw() {
    ofEnableAlphaBlending();
    
    ofDisableDepthTest();
    processor->draw();
    ofEnableDepthTest();
    
    
    if (session.currentFrame){
        if (session.currentFrame.camera){
           
            camera.begin();
            processor->setARCameraMatrices();
            
            for (int i = 0; i < session.currentFrame.anchors.count; i++){
                ARAnchor * anchor = session.currentFrame.anchors[i];

                ofPushMatrix();
                ofMatrix4x4 mat = convert<matrix_float4x4, ofMatrix4x4>(anchor.transform);
                ofMultMatrix(mat);
                
//                ofSetColor(73, 112, 19, 120);
                ofSetColor(255,180);
                ofRotate(90,0,0,1);
                
                //=============== Debug =====================
                cout << "mat.getTranslation: " << mat.getTranslation() << endl;
                ofMatrix4x4 cam = convert<matrix_float4x4, ofMatrix4x4>(session.currentFrame.camera.transform);
                cout << "Cam Pos: " << cam.getTranslation() << endl;
                //=============== Debug =====================

                if(i == 0){
                    drawMesh.draw();
                }
                ofPopMatrix();
            }
            camera.end();
        }
    }
    ofDisableDepthTest();
    // ========== DEBUG STUFF ============= //
//    processor->debugInfo.drawDebugInformation(font);
    
    // ========== GUI STUFF ============= //
    gui.draw();
    // ========== GUI STUFF ============= //
}

//--------------------------------------------------------------
void ofApp::exit() {
    //
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs &touch){
    if(noMesh == true){
        if (session.currentFrame){
            ARFrame *currentFrame = [session currentFrame];
            
            matrix_float4x4 translation = matrix_identity_float4x4;
            translation.columns[3].z = -0.2;
            matrix_float4x4 transform = matrix_multiply(currentFrame.camera.transform, translation);
            
            // Add a new anchor to the session
            ARAnchor *anchor = [[ARAnchor alloc] initWithTransform:transform];
            
            [session addAnchor:anchor];
        }
        noMesh = false;
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &inBuffer){
    inBuffer.getChannel(mltk.monoAudioBuffer, 0);
    inBuffer.getChannel(patchBuffer, 0);
}

//void ofApp::audioIn(float *input, int bufferSize, int nChannels){
//
//}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer){
    outBuffer.setChannel(patchBuffer, 0);
//    outBuffer.setChannel(patchBuffer, 1);
}

//void ofApp::audioOut(float *output, int bufferSize, int nChannels){
//    
//}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs &touch){
    
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs &touch){
    
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs &touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
  
    processor->deviceOrientationChanged(newOrientation);
}


//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs& args){
    
}


