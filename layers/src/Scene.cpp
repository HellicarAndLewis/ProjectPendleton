#include "Scene.h"


void Scene::setup() {
	box2d.init();
	box2d.setGravity(0, 10);
	//box2d.createBounds();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
	snowImage.load("assets/images/avalanche/snow.png");

	createLandscape();
}


void Scene::update() {
	box2d.update();
}



void Scene::draw(bool debug) {
	ofPushStyle();
	for (int i = 0; i<circles.size(); i++) {
		if (debug) {
			ofFill();
			ofSetHexColor(0xf6c738);
			circles[i].get()->draw();
		}
		int rad = circles[i].get()->getRadius() * 2;
		auto pos = circles[i].get()->getPosition();
		snowImage.draw(pos.x - rad, pos.y - rad, rad * 2, rad * 2);
	}

	for (int i = 0; i<edges.size(); i++) {
		edges[i].get()->draw();
	}

	if (debug) {
		for (int i = 0; i < boxes.size(); i++) {
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
	}
	ofPopStyle();
}

void Scene::birth(float scale) {
	float r = ofMap(scale, 0, 1, 6, 30);
	circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
	circles.back().get()->setPhysics(3.0, 0.53, 0.1);
	float x = ofRandom(ofGetWidth() * 0.8, ofGetWidth() * 0.95);
	circles.back().get()->setup(box2d.getWorld(), x, 0, r);
}

void Scene::createLandscape() {
	box2d.createBounds(0, 0, ofGetWidth(), ofGetHeight());

	// create the sloping ground
	shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
	edge.get()->addVertex(0, ofGetHeight());
	edge.get()->addVertex(ofGetWidth(), ofGetHeight() * 0.6);
	edge.get()->create(box2d.getWorld());
	edges.push_back(edge);

	// create wall
	int x = ofGetWidth() * 0.7;
	int y = ofGetHeight() * 0.3;
	while (y < ofGetHeight() * 0.9) {
		shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
		edge.get()->addVertex(x, y);
		y += 50;
		edge.get()->addVertex(x + 3, y);
		y += 5;
		edge.get()->create(box2d.getWorld());
		edges.push_back(edge);
	}
}

void Scene::clearSnow() {
	for (int i = 0; i<circles.size(); i++) {
		circles[i].get()->destroy();
		circles[i].reset();
	}
	circles.clear();
}


void Scene::keyPressed(int key) {
	if (key == 'c') {
		float r = ofRandom(4, 20);
		circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
		circles.back().get()->setPhysics(3.0, 0.53, 0.1);
		circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);

	}
	if (key == 'r') {
		clearSnow();
	}
}

void Scene::keyReleased(int key) {}

void Scene::mouseMoved(int x, int y) {
	for (int i = 0; i<edges.size(); i++) {
		ofRectangle wallChunk = edges[i].get()->getBoundingBox();
		if (wallChunk.width < ofGetWidth() * 0.3) {
			wallChunk.scaleFromCenter(6, 1);
			if (wallChunk.inside(ofPoint(x, y))) {
				edges[i].get()->destroy();
				edges[i].reset();
				edges.erase(edges.begin() + i);
			}
		}
	}
}

void Scene::mouseDragged(int x, int y, int button) {}

void Scene::mousePressed(int x, int y, int button) {}

void Scene::mouseReleased(int x, int y, int button) {}

void Scene::resized(int w, int h) {
	for (int i = 0; i<edges.size(); i++) {
		edges[i].get()->destroy();
		edges[i].reset();
	}
	edges.clear();
	createLandscape();
}