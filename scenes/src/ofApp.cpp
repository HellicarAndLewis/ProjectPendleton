#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetCircleResolution(100);
	ofSetVerticalSync(true);
#ifdef _WIN32
	// Turn on vertical screen sync under Windows.
	// (I.e. it uses the WGL_EXT_swap_control extension)
	typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(1);
#endif

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	mode = SINGLE_SCENE;
	doDrawKinectInputs = false;

	ofRectangle bgContainer = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
	bgImages[0].setup("assets/images/Env Background Renders/Splash0.png", bgContainer);
	bgContainer = ofRectangle(ofGetWidth(), 0, ofGetWidth(), ofGetHeight());
	bgImages[1].setup("assets/images/Env Background Renders/Env_CandyKingdom.png", bgContainer);
	bgImages[2].setup("assets/images/Env Background Renders/Env_Dongeon.png", bgContainer);
	bgImages[3].setup("assets/images/Env Background Renders/Env_Forest.png", bgContainer);
	bgImages[0].show(ofPoint(), ofPoint(), 0);

	// hit areas
	hitAreaScenePrevious.setup(ofRectangle(0,0,ofGetWidth() * 0.2, ofGetHeight()));
	hitAreaSceneNext.setup(ofRectangle(ofGetWidth() - (ofGetWidth() * 0.2), 0, ofGetWidth() * 0.2, ofGetHeight()));
	hitAreaSceneNext.listeners += [this](bool b) {
		this->nextScene();
	};
	hitAreaScenePrevious.listeners += [this](bool b) {
		this->prevScene();
	};
	hitAreaSceneNext.isEnabled = false;
	hitAreaScenePrevious.isEnabled = false;

	hitAreaSingleScene.setup("assets/images/Finn Punch/FinnCamPunch_1.png", "assets/images/Finn Punch/FinnCamPunch_2.png", 0.5);
	hitAreaSingleScene.isEnabled = false;

	hotSpots.push_back(&hitAreaScenePrevious);
	hotSpots.push_back(&hitAreaSceneNext);
	hotSpots.push_back(&hitAreaSingleScene);
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
	for (auto & bg : bgImages) {
		bg.update();
	}

	// Simple hit detection for right-hand in hit areas
	auto & bodies = kinect.getBodySource()->getBodies();
	auto coordMapper = kinect.getBodySource()->getCoordinateMapper();
	for (auto & hotSpot : hotSpots) {
		bool isHit = false;
		for (auto & body : bodies) {
			if (body.tracked) {
				auto handRightPos = body.joints.at(JointType_HandRight).getProjected(coordMapper);
				auto handRightPosScaled = ofPoint(handRightPos.x * colourToWindowScale, handRightPos.y * colourToWindowScale);
				if (hotSpot->boundsTest(handRightPosScaled)) isHit = true;
			}
		}
		if (hotSpot->boundsTest(ofPoint(ofGetMouseX(), ofGetMouseY()))) isHit = true;
		if (!isHit) hotSpot->isHit = false;
		hotSpot->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
	if (doDrawKinectInputs) drawKinectInputs();
	// BG image
	for (auto & bg : bgImages) {
		bg.draw();
	}
	// draw kinect body source
	// this is the full skeleton
	int w = kinect.getColorSource()->getWidth() * colourToWindowScale;
	int h = kinect.getColorSource()->getHeight() * colourToWindowScale;
	kinect.getBodySource()->drawProjected(0, 0, w, h);
	// draw the hit areas
	for (auto & hotSpot : hotSpots) {
		hotSpot->draw();
	}
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

void ofApp::nextScene() {
	ofLogNotice() << "ofApp::nextScene";
	if (sceneIndex < SCENE_COUNT - 1) {
		bgImages[sceneIndex].hide(ofVec2f(0, 0), ofVec2f(-ofGetWidth(), 0));
		sceneIndex++;
		bgImages[sceneIndex].show(ofVec2f(ofGetWidth(), 0), ofVec2f(0, 0));
	}
}
void ofApp::prevScene() {
	ofLogNotice() << "ofApp::prevScene";
	if (sceneIndex > 0) {
		bgImages[sceneIndex].hide(ofVec2f(0, 0), ofVec2f(ofGetWidth(), 0));
		sceneIndex--;
		bgImages[sceneIndex].show(ofVec2f(-ofGetWidth(), 0), ofVec2f(0, 0));
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		break;
	case OF_KEY_LEFT:
		if (mode == MULTIPLE_SCENES) prevScene();
		break;
	case OF_KEY_RIGHT:
		if (mode == MULTIPLE_SCENES) nextScene();
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
	if (mode == SINGLE_SCENE) {
		mode = MULTIPLE_SCENES;
		hitAreaSceneNext.isEnabled = false;
		hitAreaScenePrevious.isEnabled = false;
		hitAreaSingleScene.isEnabled = true;
	}
	else {
		mode = SINGLE_SCENE;
		if (sceneIndex != 0) {
			bgImages[sceneIndex].hide(ofVec2f(0, 0), ofVec2f(0, 0), 0);
			sceneIndex = 0;
			bgImages[sceneIndex].show(ofVec2f(0, 0), ofVec2f(0, 0), 0);
		}
		hitAreaSceneNext.isEnabled = true;
		hitAreaScenePrevious.isEnabled = true;
		hitAreaSingleScene.isEnabled = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	ofRectangle bgContainer = ofRectangle(0, 0, w, h);
	for (auto & bg : bgImages) {
		bg.resize(bgContainer);
	}

	hitAreaScenePrevious.setup(ofRectangle(0, 0, w * 0.2, h));
	hitAreaSceneNext.setup(ofRectangle(w - (w * 0.2), 0, w * 0.2, h));
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
