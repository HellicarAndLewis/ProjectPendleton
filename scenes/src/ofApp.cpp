#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	mode = SINGLE_SCENE;
	doDrawKinectInputs = false;

	bgImages[0].loadImage("assets/images/Env Background Renders/Splash0.png");
	bgImages[1].loadImage("assets/images/Env Background Renders/Env_CandyKingdom.png");
	bgImages[2].loadImage("assets/images/Env Background Renders/Env_Dongeon.png");
	bgImages[3].loadImage("assets/images/Env Background Renders/Env_Forest.png");
	hitAreaSingleScene.setup("assets/images/Finn Punch/FinnCamPunch_1.png", "assets/images/Finn Punch/FinnCamPunch_2.png", 0.5);
}

//--------------------------------------------------------------
void ofApp::update(){

	// update hit areas based on app dimenions
	int w = ofGetWidth() * 0.25;
	int h = ofGetHeight() * 0.25;
	hitAreaSingleScene.bounds.setPosition(ofGetWidth() * 0.5, ofGetHeight() - hitAreaSingleScene.bounds.getHeight());

	// scale factor for the kinect colour image to the oF window size
	colourToWindowScale = ofGetWidth() / kinect.getColorSource()->getWidth();

	kinect.update();

	// Simple hit detection for right-hand in hit areas
	if (mode == SINGLE_SCENE) {
		hitAreaSingleScene.isHit = false;
		auto & bodies = kinect.getBodySource()->getBodies();
		for (auto & body : bodies) {
			if (body.tracked) {
				auto handRightPos = body.joints.at(JointType_HandRight).getProjected(kinect.getBodySource()->getCoordinateMapper());
				auto handRightPosScaled = ofPoint(handRightPos.x * colourToWindowScale, handRightPos.y * colourToWindowScale);
				if (hitAreaSingleScene.bounds.inside(handRightPosScaled)) {
					hitAreaSingleScene.isHit = true;
				}
			}
		}
		if (hitAreaSingleScene.bounds.inside(ofPoint(ofGetMouseX(), ofGetMouseY()))) {
			hitAreaSingleScene.isHit = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (doDrawKinectInputs) drawKinectInputs();
	if (mode == SINGLE_SCENE) {
		ofBackground(255);
		// BG image
		float bgImageScale = ofGetHeight() / bgImages[0].getHeight();
		bgImages[0].draw(0, 0, bgImages[0].getWidth() * bgImageScale, bgImages[0].getHeight() * bgImageScale);
		// draw kinect body source
		// this is the full skeleton
		int w = kinect.getColorSource()->getWidth() * colourToWindowScale;
		int h = kinect.getColorSource()->getHeight() * colourToWindowScale;
		kinect.getBodySource()->drawProjected(0, 0, w, h);
		// draw the hit areas
		hitAreaSingleScene.draw();
		// Draw a circle for each tracked right hand
		auto & bodies = kinect.getBodySource()->getBodies();
		for (auto & body : bodies) {
			if (body.tracked) {
				auto handRightPos = body.joints.at(JointType_HandRight).getProjected(kinect.getBodySource()->getCoordinateMapper());
				auto handRightPosScaled = ofPoint(handRightPos.x * colourToWindowScale, handRightPos.y * colourToWindowScale);
				ofPushStyle();
				ofSetColor(0, 0, 255, 180);
				ofDrawCircle(handRightPosScaled, 20);
				ofPopStyle();
			}
		}
	}
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 20, ofGetHeight() - 20, ofColor(0, 0, 200));
}

void ofApp::drawKinectInputs() {
	int previewWidth = ofGetWidth() / 2;
	int previewHeight = previewWidth * 0.526;

	kinect.getDepthSource()->draw(0, 0, previewWidth, previewHeight);  // note that the depth texture is RAW so may appear dark

	float colorHeight = previewWidth * (kinect.getColorSource()->getHeight() / kinect.getColorSource()->getWidth());
	float colorTop = (previewHeight - colorHeight) / 2.0;
	kinect.getColorSource()->draw(previewWidth, 0 + colorTop, previewWidth, colorHeight);
	kinect.getBodySource()->drawProjected(previewWidth, 0 + colorTop, previewWidth, colorHeight);

	kinect.getInfraredSource()->draw(0, previewHeight, previewWidth, previewHeight);

	kinect.getBodyIndexSource()->draw(previewWidth, previewHeight, previewWidth, previewHeight);
	kinect.getBodySource()->drawProjected(previewWidth, previewHeight, previewWidth, previewHeight, ofxKFW2::ProjectionCoordinates::DepthCamera);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
