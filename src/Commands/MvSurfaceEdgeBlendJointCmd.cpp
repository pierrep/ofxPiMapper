#include "MvSurfaceEdgeBlendJointCmd.h"

namespace ofx {
namespace piMapper {

MvSurfaceEdgeBlendJointCmd::MvSurfaceEdgeBlendJointCmd(int jointIndex, ProjectionEditorWidget * p){
    _jointIndex = jointIndex;
    _projectionEditorWidget = p;
}

void MvSurfaceEdgeBlendJointCmd::exec(){
    ofLogNotice("MvSurfaceEdgeBlendJointCmd", "exec");
    _prevJointIndex = _projectionEditorWidget->getSelectedEdgeBlendJoint();
    if(_prevJointIndex != -1) {
        _projectionEditorWidget->selectEdgeBlendJoint(_jointIndex);
    }

    _prevJointPos = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->getDragPosition();
    float x = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->position.x;
    float y = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->position.y;
    ofLogNotice("MvSurfaceEdgeBlendJointCmd") << "position -> x: " << x << " y: " << y << " dragPosion -> x:" << _prevJointPos.x << " y:" << _prevJointPos.y;
}

void MvSurfaceEdgeBlendJointCmd::undo(){
	ofLogNotice("MvSurfaceVertCommand", "undo");
    _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->setDragPosition(_prevJointPos);
    float x = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->position.x;
    float y = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->position.y;
    _projectionEditorWidget->setBlendEdges();
    ofLogNotice("MvSurfaceEdgeBlendJointCmd Undo") << "position -> x: " << x << " y: " << y << " dragPosion -> x:" << _prevJointPos.x << " y:" << _prevJointPos.y;

    if(_prevJointIndex != -1) {
        _projectionEditorWidget->selectEdgeBlendJoint(_prevJointIndex);
    }
}

} // namespace piMapper
} // namespace ofx

