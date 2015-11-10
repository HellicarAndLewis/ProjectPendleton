#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class SnowBall : public ofxBox2dCircle {
public:
	void update() {
		if (isMelting) {
			color.lerp(colorMelted, 0.1);
			if (color.g < 124) this->destroy();
		}
	}
	void melt() {
		isMelting = true;
	}
	void draw(bool debug = false) {
		if (debug) {
			ofSetHexColor(0xf6c738);
			ofxBox2dCircle::draw();
		}
		else {
			ofSetColor(color);
			int rad = getRadius() * 2;
			auto pos = getPosition();
			ofRectangle quad = ofRectangle(pos.x - rad, pos.y - rad, rad * 2, rad * 2);
			ofRectangle text = ofRectangle(0, 0, 60, 60);
			glBegin(GL_QUADS);
			// bottom left
			drawTexCoord(text.getBottomLeft());
			drawVertex(quad.getBottomLeft());
			// top left
			drawTexCoord(text.getTopLeft());
			drawVertex(quad.getTopLeft());
			// top right
			drawTexCoord(text.getTopRight());
			drawVertex(quad.getTopRight());
			// bottom right
			drawTexCoord(text.getBottomRight());
			drawVertex(quad.getBottomRight());
			glEnd();
		}
	}
	void drawTexCoord(ofVec2f point) {
		glTexCoord2f(point.x, point.y);
	}

	void drawVertex(ofVec2f point) {
		glVertex3f(point.x, point.y, 0);
	}

	bool isMelting = false;
	float meltAmount = 0.0f;
	ofColor color = ofColor(255);
	ofColor colorMelted = ofColor(255, 122, 134);
};

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
	vector <shared_ptr<SnowBall>> circles;
	vector <shared_ptr<ofxBox2dRect>> boxes;
	vector <shared_ptr<ofxBox2dEdge>> edges;

	ofImage snowImage;

};

