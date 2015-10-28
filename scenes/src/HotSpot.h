#pragma once
#include "ofMain.h"
#include "ofxLiquidEvent.h"

class HotSpot {
public:
	enum Type {
		IMAGE, HIDDEN
	} type;

	void setup(ofRectangle& bounds);
	void setup(string imgPathOff, string imgPathOn, float scale = 1.0);
	void update();
	void draw(int x, int y);
	void draw();

	bool boundsTest(ofPoint& p);

	ofxLiquidEvent<bool> listeners;

	ofRectangle bounds;
	ofImage imageOff;
	ofImage imageOn;
	float imageScale;
	bool isHit;
	int hitCount = 0;
	int hitTarget = 60;
	bool isDebugDraw = true;
	bool isEnabled = true;
};
