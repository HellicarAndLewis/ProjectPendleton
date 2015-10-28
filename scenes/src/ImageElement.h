//
//  ImageElement.h
//  show
//
//  Created by Chris Mullany on 17/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "Sequencable.h"

//
// Image with fadein/out
//
class ImageElement : public Sequencable {
public:
    ImageElement();
	void setup(string path, ofRectangle& container);
	void update();
    void draw();

	void resize(ofRectangle& container);
    void show(ofVec2f from, ofVec2f to, float duration = 1.0f, float delay = 0.0f);
    void hide(ofVec2f from, ofVec2f to, float duration = 1.0f);
    
    ofImage image;

protected:  
private:
	ofParameter<ofVec2f> pos;
	ofRectangle container;
    
};