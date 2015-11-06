#pragma once

#include "ofMain.h"
#include "ImageElement.h"
#include "ofxGui.h" 
#include "Scene.h"
#include "Mic.h"
#include "Vision.h"

// TODO:
// optimise particle drawing with billboard shader
// make the snow melt more dramatic

class ofApp : public ofBaseApp{

	public:

		enum Mode {
			MODE_MIC, MODE_FLOW, MODE_SKELETON
		} mode;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxPanel gui;
		ImageElement bgImage;
		Vision vision;
		Scene scene;
		Mic mic;
};
