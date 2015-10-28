#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxAssets.h"
#include "HotSpot.h"

// TODO:
// hit area into seperate file
// add images using ofxassets

class ofApp : public ofBaseApp{

	public:
		enum Mode {
			SINGLE_SCENE, MULTIPLE_SCENES
		} mode;

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

		bool doDrawKinectInputs;
		HotSpot hitAreaSingleScene;
		HotSpot hitAreaSceneNext;
		float colourToWindowScale;

		ofImage bgImages[4];


};
