#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

class Scene {

public:

	void setup();
	void update();
	void draw(bool debug = false);
	void birth(float scale = 1.0f);
	void createLandscape();
	void clearSnow();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);

	ofxBox2d box2d;
	vector <shared_ptr<ofxBox2dCircle>> circles;
	vector <shared_ptr<ofxBox2dRect>> boxes;
	vector <shared_ptr<ofxBox2dEdge>> edges;

	ofImage snowImage;

};

