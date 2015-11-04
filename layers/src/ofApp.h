#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "HotSpot.h"
#include "ImageElement.h"
#include "ofxCv.h"
#include "ofxGui.h"

#define SCENE_COUNT 4

class ofApp : public ofBaseApp{

	public:
		enum Mode {
			SINGLE_SCENE, MULTIPLE_SCENES
		} mode;

		void setup();
		void update();
		void draw();
		void drawKinectInputs();

		void nextScene();
		void prevScene();

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
		HotSpot hitAreaSingleScene;
		HotSpot hitAreaSceneNext;
		HotSpot hitAreaScenePrevious;
		vector<HotSpot*> hotSpots;
		float colourToWindowScale;

		ImageElement bgImages[SCENE_COUNT];
		int sceneIndex = 0;


};
