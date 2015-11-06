#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinectForWindows2.h"
#include "ofxGui.h"

class Vision : public ofBaseApp {

public:

	void setup();
	void update(bool doFlow = true, bool doSkeleton = false);
	void draw(bool doFlow = true, bool doSkeleton = false);
	void drawKinectInputs();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);

	ofxKFW2::Device kinect;
	bool doDrawKinectInputs = false;
	bool doDrawSmallColour = false;
	float colourToWindowScale;

	// CV
	ofxCv::FlowFarneback fb;
	//ofxCv::FlowPyrLK lk;
	//ofxCv::Flow* curFlow;
	ofxCv::Mat flowMat;
	bool flowMatAllocated = false;
	bool isFrameNew = false;


	// CV GUI
	ofParameterGroup paramGroup;
	ofParameter<float> fbPyrScale, lkQualityLevel, fbPolySigma;
	ofParameter<int> fbLevels, lkWinSize, fbIterations, fbPolyN, fbWinSize, lkMaxLevel, lkMaxFeatures, lkMinDistance;
	ofParameter<bool> fbUseGaussian, usefb;

};
