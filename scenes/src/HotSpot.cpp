#include "HotSpot.h"

void HotSpot::setup() {

}


void HotSpot::setup(string imgPathOff, string imgPathOn, float scale) {
	imageOff.load(imgPathOff);
	imageOn.load(imgPathOn);
	type = IMAGE;
	imageScale = scale;
	bounds.setSize(imageOn.getWidth() * imageScale, imageOn.getHeight() * imageScale);
}

void HotSpot::update() {

}


void HotSpot::draw(int x, int y) {
	bounds.setPosition(x, y);
	draw();
}

void HotSpot::draw() {
	if (type == IMAGE) {
		if (isHit) imageOn.draw(bounds.getTopLeft(), imageOn.getWidth() * imageScale, imageOn.getHeight() * imageScale);
		else imageOff.draw(bounds.getTopLeft(), imageOff.getWidth() * imageScale, imageOff.getHeight() * imageScale);
	}
	if (isDebugDraw) {
		ofPushStyle();
		if (isHit) ofSetColor(0, 200, 0, 60);
		else ofSetColor(200, 0, 0, 60);
		ofDrawRectangle(bounds);
		ofPopStyle();
	}
}