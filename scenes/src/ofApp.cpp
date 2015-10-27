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
}

//--------------------------------------------------------------
void ofApp::update(){

	// update hit areas based on app dimenions
	int w = ofGetWidth() * 0.25;
	int h = ofGetHeight() * 0.25;
	hitAreaSingleScene.bounds.set(ofGetWidth() - w, 0, w, h);

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
