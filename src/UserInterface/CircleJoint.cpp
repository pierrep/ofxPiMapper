#include "CircleJoint.h"

namespace ofx {
namespace piMapper {

CircleJoint::CircleJoint(){
	setDefaultProperties();

    circle.load("circle-hollow.png");
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
	}else{
		ofSetColor(fillColor);
	}

	#if (OF_VERSION_MAJOR == 0 && OF_VERSION_MINOR >= 9) || OF_VERSION_MAJOR > 0
        if(selected){
            if(fillColorSelected.a != 0) {
                //ofDrawCircle(position.x, position.y, radius);
                circle.draw(position.x,position.y);
            }
        } else {
            if(fillColor.a != 0) {
                //ofDrawCircle(position.x, position.y, radius);
                circle.draw(position.x,position.y);
            }
        }
	#else
        ofCircle(position.x, position.y, radius);
	#endif
	ofNoFill();

	if(selected){
		ofSetColor(strokeColorSelected);
	}else{
		ofSetColor(strokeColor);
	}

	ofSetLineWidth(strokeWidth);
	#if (OF_VERSION_MAJOR == 0 && OF_VERSION_MINOR >= 9) || OF_VERSION_MAJOR > 0
        if(selected){
            if(strokeColorSelected.a != 0) {
                //ofDrawCircle(position.x, position.y, radius);
                circle.draw(position.x,position.y);
            }
        } else {
            if(strokeColor.a != 0) {
                //ofDrawCircle(position.x, position.y, radius);
                circle.draw(position.x,position.y);
            }
        }
	#else
		ofCircle(position.x, position.y, radius);
	#endif
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
