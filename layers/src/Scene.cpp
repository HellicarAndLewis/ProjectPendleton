#include "Scene.h"


void Scene::setup() {
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createBounds();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
}


void Scene::update() {
	box2d.update();
}



void Scene::draw() {
	ofPushStyle();
	for (int i = 0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		circles[i].get()->draw();
	}

	for (int i = 0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].get()->draw();
	}

	// draw the ground
	box2d.drawGround();

	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: " + ofToString(box2d.getBodyCount()) + "\n";
	info += "Total Joints: " + ofToString(box2d.getJointCount()) + "\n\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
	ofPopStyle();
}

void Scene::birth(float scale) {
	float r = scale * 100;
	circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
	circles.back().get()->setPhysics(3.0, 0.53, 0.1);
	circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
}


void Scene::keyPressed(int key) {

	if (key == 'c') {
		float r = ofRandom(4, 20);
		circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
		circles.back().get()->setPhysics(3.0, 0.53, 0.1);
		circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);

	}

	if (key == 'b') {
		float w = ofRandom(4, 20);
		float h = ofRandom(4, 20);
		boxes.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
		boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
		boxes.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), w, h);
	}

}

void Scene::keyReleased(int key) {}

void Scene::mouseMoved(int x, int y) {}

void Scene::mouseDragged(int x, int y, int button) {}

void Scene::mousePressed(int x, int y, int button) {}

void Scene::mouseReleased(int x, int y, int button) {}

void Scene::resized(int w, int h) {
	box2d.createBounds(0, 0, w, h);
}