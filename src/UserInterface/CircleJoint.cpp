#include "CircleJoint.h"

namespace ofx {
namespace piMapper {

CircleJoint::CircleJoint(){
	setDefaultProperties();

    circle.load("circle-hollow.png");
    circle_filled.load("circle-filled.png");
}

void CircleJoint::update(){
	if(!enabled){
		return;
	}
}

void CircleJoint::draw(){
	if(!visible){
		return;
	}
	
	if(!enabled){
		return;
	}
	
	ofEnableAlphaBlending();

	ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
	ofFill();

    if(selected){
        ofSetColor(fillColorSelected);
        if(fillColorSelected.a != 0) {
            circle_filled.draw(position.x,position.y);
        }
    } else {
        ofSetColor(fillColor);
        if(fillColor.a != 0) {
            circle_filled.draw(position.x,position.y);
        }
    }
	ofNoFill();

	ofSetLineWidth(strokeWidth);

    if(selected){
        ofSetColor(strokeColorSelected);
        if(strokeColorSelected.a != 0) {
            circle.draw(position.x,position.y);
        }
    } else {
        ofSetColor(strokeColor);
        if(strokeColor.a != 0) {
            circle.draw(position.x,position.y);
        }
    }
	ofPopStyle();
}

void CircleJoint::setDefaultProperties(){
	radius = 10.0f;
}

bool CircleJoint::hitTest(Vec2 pos){
	float distance = position.distance(pos);
	if(distance < radius){
		return true;
	}else{
		return false;
	}
}

} // namespace piMapper
} // namespace ofx
