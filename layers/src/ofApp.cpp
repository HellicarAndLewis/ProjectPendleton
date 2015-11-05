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
	doDrawKinectInputs = false;

	ofRectangle bgContainer = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
	bgImage.setup("assets/images/Env Background Renders/Splash0.png", bgContainer);
	bgImage.show(ofPoint(), ofPoint(), 0);

	scene.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

	// scale factor for the kinect colour image to the oF window size
	colourToWindowScale = ofGetWidth() / kinect.getColorSource()->getWidth();
	bgImage.update();
	kinect.update();
	scene.update();

	// limb tracking
	auto & bodies = kinect.getBodySource()->getBodies();
	auto coordMapper = kinect.getBodySource()->getCoordinateMapper();
	for (auto & body : bodies) {
		if (body.tracked) {
			auto handRightPos = body.joints.at(JointType_HandRight).getProjected(coordMapper);
			auto handRightPosScaled = ofPoint(handRightPos.x * colourToWindowScale, handRightPos.y * colourToWindowScale);
			// do something with hand
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
	if (doDrawKinectInputs) drawKinectInputs();

	// draw things
	bgImage.draw();
	scene.draw();

	// draw kinect body source
	// this is the full skeleton
	int w = kinect.getColorSource()->getWidth() * colourToWindowScale;
	int h = kinect.getColorSource()->getHeight() * colourToWindowScale;
	kinect.getBodySource()->drawProjected(0, 0, w, h);

	if (doDrawSmallColour) {
		kinect.getColorSource()->draw(10, 10, ofGetWidth()*0.2, ofGetWidth()*0.2*0.526);
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
	case 'd':
		doDrawSmallColour = !doDrawSmallColour;
		break;
	default:
		break;
	}
	scene.keyPressed(key);
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
	ofRectangle bgContainer = ofRectangle(0, 0, w, h);
	bgImage.resize(bgContainer);
	scene.resized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
