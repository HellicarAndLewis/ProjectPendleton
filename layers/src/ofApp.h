#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ImageElement.h"
#include "ofxGui.h" 
#include "Scene.h"
#include "Mic.h"

// TODO:
// move kinect to Vision, implement ofxCv optical flow
// on update, check for flow near wall sections = destroy wall sections
// check for kinect hands contact with snow = melt snow? Or kick it?

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawKinectInputs();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxKFW2::Device kinect;

		bool doDrawKinectInputs = false;
		bool doDrawSmallColour = false;
		float colourToWindowScale;

		ImageElement bgImage;
		Scene scene;
		Mic mic;
};
