#include "Vision.h"
using namespace ofxCv;
using namespace cv;

void Vision::setup() {
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initBodySource();

	// GUI params
	paramGroup.setName("Vision");
	paramGroup.add(fbPyrScale.set("fbPyrScale", 0, 0, .99));
	paramGroup.add(fbLevels.set("fbLevels", 1, 1, 8));
	paramGroup.add(fbIterations.set("fbIterations", 1, 1, 8));
	paramGroup.add(fbPolyN.set("fbPolyN", 7, 5, 10));
	paramGroup.add(fbPolySigma.set("fbPolySigma", 1.5, 1.1, 2));
	paramGroup.add(fbUseGaussian.set("fbUseGaussian", false));
	paramGroup.add(fbWinSize.set("winSize", 8, 4, 64));

}


void Vision::update(bool doFlow, bool doSkeleton) {

	if (!doFlow && !doSkeleton) return;

	kinect.update();

	// scale factor for the kinect colour image to the oF window size
	colourToWindowScale = ofGetWidth() / kinect.getColorSource()->getWidth();
	isFrameNew = kinect.getDepthSource()->isFrameNew();

	if (isFrameNew && doFlow) {
		ofPixels& colourPixels = kinect.getColorSource()->getPixels();
		if (colourPixels.getWidth() > 0) {
			// update flow settings
			fb.setPyramidScale(fbPyrScale);
			fb.setNumLevels(fbLevels);
			fb.setWindowSize(fbWinSize);
			fb.setNumIterations(fbIterations);
			fb.setPolyN(fbPolyN);
			fb.setPolySigma(fbPolySigma);
			fb.setUseGaussian(fbUseGaussian);
			// do FB based flow
			resize(colourPixels, flowMat, 0.07, 0.07);
			fb.calcOpticalFlow(flowMat);
		}
	}
}


void Vision::draw(bool doFlow, bool doSkeleton) {
	
	if (doDrawKinectInputs) drawKinectInputs();

	if (doSkeleton) {
		// draw kinect body source
		// this is the full skeleton
		int w = kinect.getColorSource()->getWidth() * colourToWindowScale;
		int h = kinect.getColorSource()->getHeight() * colourToWindowScale;
		kinect.getBodySource()->drawProjected(0, 0, w, h);
	}

	if (doDrawSmallColour) {
		kinect.getColorSource()->draw(10, 10, ofGetWidth()*0.2, ofGetWidth()*0.2*0.526);
	}

	if (doFlow && fb.getHeight() > 0) {
		ofPushStyle();
		ofSetLineWidth(4);
		float scale = ofGetHeight() / fb.getHeight();
		ofSetColor(0);
		ofSetColor(200, 0, 0);
		fb.draw(0, 0, fb.getWidth() * scale, fb.getHeight() * scale);
		ofPopStyle();
	}
}

void Vision::drawKinectInputs() {
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


void Vision::keyPressed(int key) {
	switch (key) {
	case 'd':
		doDrawSmallColour = !doDrawSmallColour;
		break;
	default:
		break;
	}
}


void Vision::keyReleased(int key) {

}


void Vision::mouseMoved(int x, int y) {

}


void Vision::mouseDragged(int x, int y, int button) {

}


void Vision::mousePressed(int x, int y, int button) {

}


void Vision::mouseReleased(int x, int y, int button) {

}


void Vision::mouseEntered(int x, int y) {

}


void Vision::mouseExited(int x, int y) {

}


void Vision::windowResized(int w, int h) {

}