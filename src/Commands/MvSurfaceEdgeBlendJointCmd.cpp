#include "MvSurfaceEdgeBlendJointCmd.h"

namespace ofx {
namespace piMapper {

MvSurfaceEdgeBlendJointCmd::MvSurfaceEdgeBlendJointCmd(int jointIndex, ProjectionEditorWidget * p){
    _jointIndex = jointIndex;
    _projectionEditorWidget = p;
}

void MvSurfaceEdgeBlendJointCmd::exec(){
    ofLogVerbose("MvSurfaceEdgeBlendJointCmd", "exec");
    _prevJointIndex = _projectionEditorWidget->getSelectedEdgeBlendJoint();
    if(_prevJointIndex != -1) {
        _projectionEditorWidget->selectEdgeBlendJoint(_jointIndex);
    }

    _prevJointPos = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->getDragPosition();
    //float x = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->position.x;
    //float y = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->position.y;
}

void MvSurfaceEdgeBlendJointCmd::undo(){
    _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->setDragPosition(_prevJointPos);
    _projectionEditorWidget->setBlendEdges();

    if(_prevJointIndex != -1) {
        _projectionEditorWidget->selectEdgeBlendJoint(_prevJointIndex);
    }
}

} // namespace piMapper
} // namespace ofx

