#include "EdgeBlendJoint.h"

namespace ofx {
namespace piMapper {

EdgeBlendJoint::EdgeBlendJoint(){
	setDefaultProperties();
    setDefaultColors();
}

void EdgeBlendJoint::update(){
	if(!enabled){
		return;
	}
}

void EdgeBlendJoint::draw(){
	if(!visible){
		return;
	}
	
	if(!enabled){
		return;
	}
	
	ofEnableAlphaBlending();

	ofPushStyle();
	ofFill();

	if(selected){
		ofSetColor(fillColorSelected);
	}else{
		ofSetColor(fillColor);
	}

	#if (OF_VERSION_MAJOR == 0 && OF_VERSION_MINOR >= 9) || OF_VERSION_MAJOR > 0
		ofDrawCircle(position.x, position.y, radius);
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
        ofDrawCircle(position.x, position.y, radius);
    #else
        ofCircle(position.x, position.y, radius);
    #endif
	ofPopStyle();
}

void EdgeBlendJoint::setDefaultProperties(){
	radius = 10.0f;
}

const Vec2 EdgeBlendJoint::getDragPosition(){
    return dragPosition;
}

void EdgeBlendJoint::setDragPosition(Vec2 pos){
    dragPosition = pos;
}

void EdgeBlendJoint::mouseDragged(ofMouseEventArgs & args){
    if(!bDrag){
        return;
    }
    dragPosition = Vec2(args.x, args.y) + clickDistance;
}

bool EdgeBlendJoint::hitTest(Vec2 pos){
	float distance = position.distance(pos);
	if(distance < radius){
		return true;
	}else{
		return false;
	}
}

void EdgeBlendJoint::setDefaultColors(){
    fillColor = ofColor(128, 128, 128, 255);
    strokeColor = ofColor(36, 138, 63, 255);
    fillColorSelected = ofColor(36, 138, 63, 255);
    strokeColorSelected = ofColor(36, 138, 63, 255);
}

} // namespace piMapper
} // namespace ofx
