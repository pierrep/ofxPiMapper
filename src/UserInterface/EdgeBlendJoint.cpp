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

void EdgeBlendJoint::mouseDragged(ofMouseEventArgs & args){
    if(!bDrag){
        return;
    }
    dragPosition = Vec2(args.x, args.y) + clickDistance;
    //ofLogNotice() << "Edge Blend Joint dragging...  x: " << dragPosition.x << " y: " << dragPosition.y;
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
    fillColor = ofColor(0, 255, 255, 0);
    strokeColor = ofColor(0, 255, 0);
    fillColorSelected = ofColor(255, 255, 0, 0);
    strokeColorSelected = ofColor(255, 0, 0);
}

} // namespace piMapper
} // namespace ofx
