#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

// TODO:
// hit area into seperate file
// add images using ofxassets

class ofApp : public ofBaseApp{

	public:
		enum Mode {
			SINGLE_SCENE, MULTIPLE_SCENES
		} mode;

		struct HitArea {
			ofRectangle bounds;
			bool isHit;
			void draw() {
				ofPushStyle();
				if (isHit) ofSetColor(0, 200, 0, 60);
				else ofSetColor(200, 0, 0, 60);
				ofDrawRectangle(bounds);
				ofPopStyle();
			}
		};

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
		HitArea hitAreaSingleScene;
		HitArea hitAreaSceneNext;
		float colourToWindowScale;
};
