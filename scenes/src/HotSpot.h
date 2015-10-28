#pragma once
#include "ofMain.h"

class HotSpot {
public:
	enum Type {
		IMAGE, HIDDEN
	} type;

	void setup();
	void setup(string imgPathOff, string imgPathOn, float scale = 1.0);
	void update();
	void draw(int x, int y);
	void draw();

	ofRectangle bounds;
	ofImage imageOff;
	ofImage imageOn;
	float imageScale;
	bool isHit;
	bool isDebugDraw = false;
};
