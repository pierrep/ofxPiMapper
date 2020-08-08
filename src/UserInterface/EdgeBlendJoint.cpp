#include "EdgeBlendJoint.h"

namespace ofx {
namespace piMapper {

EdgeBlendJoint::EdgeBlendJoint(){
	setDefaultProperties();
    setDefaultColors();
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

void EdgeBlendJoint::setDefaultColors(){
    fillColor = ofColor(128, 128, 128, 255);
    strokeColor = ofColor(36, 138, 63, 255);
    fillColorSelected = ofColor(36, 138, 63, 255);
    strokeColorSelected = ofColor(36, 138, 63, 255);
}

} // namespace piMapper
} // namespace ofx
