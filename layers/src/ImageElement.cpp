//
//  ImageElement.cpp
//  show
//
//  Created by Chris Mullany on 17/09/2015.
//
//

#include "ImageElement.h"

ImageElement::ImageElement() {
}

void ImageElement::setup(string path, ofRectangle& container) {
    image.loadImage(path);
	resize(container);
}

void ImageElement::update() {
	Sequencable::update();
	if (state == INTERACTIVE) {
		container.x = 0;
	}
	else {
		container.x = ofMap(progress, 0.0f, 1.0f, pos.getMin().x, pos.getMax().x, false);
	}
}

void ImageElement::draw() {
    if (state == INACTIVE) return;
    /*else if (state == INTRO)  ofSetColor(255, 255, 255, progress*255);
    else if (state == OUTRO) ofSetColor(255, 255, 255, (1-progress)*255);
    else ofSetColor(255);*/
    image.draw(container.getTopLeft(), container.width, container.height);
    ofSetColor(255);
}


void ImageElement::resize(ofRectangle& container) {
	this->container = ofRectangle(0, 0, image.getWidth(), image.getHeight());
	this->container.scaleTo(container);
}

void ImageElement::show(ofVec2f from, ofVec2f to, float duration, float delay) {
	pos.setMin(from);
	pos.setMax(to);
    timeIn = duration;
	setState(INTRO, delay);
}

void ImageElement::hide(ofVec2f from, ofVec2f to, float duration) {
	pos.setMin(from);
	pos.setMax(to);
	timeOut = duration;
	setState(OUTRO);
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////
