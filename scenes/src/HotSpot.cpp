#include "HotSpot.h"

void HotSpot::setup(ofRectangle& bounds) {
	this->bounds.set(bounds);
	type = HIDDEN;
	hitTarget = 60;
}

void HotSpot::setup(string imgPathOff, string imgPathOn, float scale) {
	imageOff.load(imgPathOff);
	imageOn.load(imgPathOn);
	type = IMAGE;
	hitTarget = 1;
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
	if (!isEnabled) return;
	if (type == IMAGE) {
		if (isHit) imageOn.draw(bounds.getTopLeft(), imageOn.getWidth() * imageScale, imageOn.getHeight() * imageScale);
		else imageOff.draw(bounds.getTopLeft(), imageOff.getWidth() * imageScale, imageOff.getHeight() * imageScale);
	}
	else {
		ofPushStyle();
		if (isHit) ofSetColor(0, 0);
		else if (hitCount > 0) ofSetColor(255, 100);
		else ofSetColor(0, 0);
		float rad = ofMap(hitCount, 0, hitTarget, 0, (bounds.getWidth() / 3));
		ofDrawCircle(bounds.getCenter(), rad);
		ofPopStyle();
	}
}


bool HotSpot::boundsTest(ofPoint& p) {
	if (!isEnabled) return false;
	bool isHitNow = bounds.inside(p);
	if (isHitNow){
		if (!isHit && hitCount++ > hitTarget) {
			isHit = true;
			hitCount = 0;
			bool b = true;
			listeners.notifyListeners(b);
		}
	}
	return isHitNow;
}