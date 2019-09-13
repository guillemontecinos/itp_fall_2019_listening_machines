#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
    
        ofSoundStream soundStream;
        ofColor backgroundColor;
        int sampleRate;
        int frameSize;
    
        vector<float> leftBuffer;
        vector<float> rightBuffer;
        vector<float> audioBuffer;
        vector<float> audioBuffer150;
        vector<float> audioBuffer300;
        vector<float> audioBuffer500;
		
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
		
        void audioIn(ofSoundBuffer &inBuffer);
        void audioOut(ofSoundBuffer &outBuffer);
};
