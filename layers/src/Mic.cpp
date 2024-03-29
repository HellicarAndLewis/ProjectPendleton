#include "Mic.h"


void Mic::setup() {
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)

	soundStream.printDeviceList();

	auto soundDevices = soundStream.getDeviceList();
	int id = 0;
	for (auto & device : soundDevices) {
		if (ofIsStringInString(device.name, "Internal")) {
			id = device.deviceID;
		}
	}
	soundStream.setDeviceID(id);


	int bufferSize = 256;

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);

	bufferCounter = 0;
	drawCounter = 0;
	smoothedVol = 0.0;
	scaledVol = 0.0;

	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}


void Mic::update() {
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back(scaledVol);

	//if we are bigger the the size we want to record - lets drop the oldest value
	if (volHistory.size() >= 400) {
		volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
	}
}


void Mic::draw() {

	ofSetColor(225);

	ofNoFill();

	// draw the left channel:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 170, 0);

	ofSetColor(225);
	ofDrawBitmapString("Left Channel", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 512, 200);

	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);

	ofBeginShape();
	for (unsigned int i = 0; i < left.size(); i++) {
		ofVertex(i * 2, 100 - left[i] * 180.0f);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(32, 370, 0);

	ofSetColor(225);
	ofDrawBitmapString("Right Channel", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 512, 200);

	ofSetColor(245, 58, 135);
	ofSetLineWidth(3);

	ofBeginShape();
	for (unsigned int i = 0; i < right.size(); i++) {
		ofVertex(i * 2, 100 - right[i] * 180.0f);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	// draw the average volume:
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(565, 170, 0);

	ofSetColor(225);
	ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
	ofDrawRectangle(0, 0, 400, 400);

	ofSetColor(245, 58, 135);
	ofFill();
	ofDrawCircle(200, 200, scaledVol * 190.0f);

	//lets draw the volume history as a graph
	ofBeginShape();
	for (unsigned int i = 0; i < volHistory.size(); i++) {
		if (i == 0) ofVertex(i, 400);

		ofVertex(i, 400 - volHistory[i] * 70);

		if (i == volHistory.size() - 1) ofVertex(i, 400);
	}
	ofEndShape(false);

	ofPopMatrix();
	ofPopStyle();

	drawCounter++;

	ofSetColor(225);
	string reportString = "buffers received: " + ofToString(bufferCounter) + "\ndraw routines called: " + ofToString(drawCounter) + "\nticks: " + ofToString(soundStream.getTickCount());
	ofDrawBitmapString(reportString, 32, 589);

}


void Mic::audioIn(float * input, int bufferSize, int nChannels) {

	float curVol = 0.0;

	// samples are "interleaved"
	int numCounted = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++) {
		left[i] = input[i * 2] * 0.5;
		right[i] = input[i * 2 + 1] * 0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted += 2;
	}

	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;

	// this is how we get the root of rms :) 
	curVol = sqrt(curVol);

	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;

	bufferCounter++;

}


void Mic::keyPressed(int key) {
	if (key == 's') {
		soundStream.start();
	}

	if (key == 'e') {
		soundStream.stop();
	}
}


void Mic::keyReleased(int key) {

}


void Mic::mouseMoved(int x, int y) {

}


void Mic::mouseDragged(int x, int y, int button) {

}


void Mic::mousePressed(int x, int y, int button) {

}


void Mic::mouseReleased(int x, int y, int button) {

}


void Mic::mouseEntered(int x, int y) {

}


void Mic::mouseExited(int x, int y) {

}


void Mic::windowResized(int w, int h) {

}


void Mic::gotMessage(ofMessage msg) {

}


void Mic::dragEvent(ofDragInfo dragInfo) {

}

