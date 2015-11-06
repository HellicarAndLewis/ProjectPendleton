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

	ofRectangle bgContainer = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
	bgImage.setup("assets/images/Env Background Renders/Splash0.png", bgContainer);
	bgImage.show(ofPoint(), ofPoint(), 0);

	vision.setup();
	scene.setup();
	mic.setup();
	mode = MODE_MIC;

	gui.setup("Settings");
	gui.add(vision.paramGroup);
}

//--------------------------------------------------------------
void ofApp::update(){

	vision.update(mode == MODE_FLOW, mode == MODE_SKELETON);
	bgImage.update();
	scene.update();
	mic.update();

	if (mic.scaledVol > 0.1){
		scene.birth(mic.scaledVol);
	}

	if (mode == MODE_SKELETON) {
		float colourToWindowScale = ofGetWidth() / vision.kinect.getColorSource()->getWidth();
		auto & bodies = vision.kinect.getBodySource()->getBodies();
		auto coordMapper = vision.kinect.getBodySource()->getCoordinateMapper();
		bool isHit = false;
		ofRectangle snowBounds;
		for (int i = 0; i<scene.circles.size(); i++) {
			for (auto & body : bodies) {
				if (body.tracked) {
					auto handRightPos = body.joints.at(JointType_HandLeft).getProjected(coordMapper);
					auto handRightPosScaled = ofPoint(handRightPos.x * colourToWindowScale, handRightPos.y * colourToWindowScale);
					snowBounds.setFromCenter(
						scene.circles[i].get()->getPosition(),
						scene.circles[i].get()->getRadius() * 4,
						scene.circles[i].get()->getRadius() * 4);
					if (snowBounds.inside(handRightPosScaled)) {
						scene.circles[i].get()->destroy();
						//scene.circles[i].reset();
					}
				}
			}
			
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);

	// draw things
	bgImage.draw();
	scene.draw();
	if (mode == MODE_MIC) mic.draw();
	vision.draw(mode == MODE_FLOW, mode == MODE_SKELETON);

	if (mode == MODE_FLOW) {
		// Flooooooow
		ofxCv::FlowFarneback* flow = &vision.fb;
		if (flow->getHeight() > 0) {
			ofPushStyle();
			ofFill();
			int stepSize = 4;
			int ySteps = flow->getHeight() / stepSize;
			int xSteps = flow->getWidth() / stepSize;
			float scale = ofGetHeight() / flow->getHeight();
			if (vision.isFrameNew) {
				int i = 0;
				float threshold = 0.1;
				for (int y = 1; y + 1 < ySteps; y++) {
					for (int x = 1; x + 1 < xSteps; x++) {
						int i = y * xSteps + x;
						ofVec2f position(x * stepSize, y * stepSize);
						ofRectangle area(position - ofVec2f(stepSize, stepSize) / 2, stepSize, stepSize);
						ofVec2f offset = flow->getAverageFlowInRegion(area);
						if (offset.lengthSquared() > threshold) {
							area.scale(scale, scale);
							area.x *= scale;
							area.y *= scale;
							for (auto wallChunk : scene.edges) {
								ofRectangle wallRect = wallChunk->getBoundingBox();
								if (wallRect.width < ofGetWidth() * 0.3 && area.intersects(wallRect)) {
									wallChunk->destroy();
								}
							}
						}
						i++;
					}
				}
			}
			ofPopStyle();
		}
	}

	// GUI
	gui.draw();
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 20, ofGetHeight() - 20, ofColor(0, 0, 200));
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
	vision.keyPressed(key);
	scene.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	scene.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (mode == MODE_MIC) mode = MODE_FLOW;
	else if (mode == MODE_FLOW) mode = MODE_SKELETON;
	else if (mode == MODE_SKELETON) mode = MODE_MIC;
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
