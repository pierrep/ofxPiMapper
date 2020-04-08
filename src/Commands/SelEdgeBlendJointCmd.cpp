#include "SelEdgeBlendJointCmd.h"

namespace ofx {
namespace piMapper {

SelEdgeBlendJointCmd::SelEdgeBlendJointCmd(ProjectionEditorWidget * p, int i){
    _projectionEditorWidget = p;
    _newJointIndex = i;
}

void SelEdgeBlendJointCmd::exec(){
    _prevJointIndex = _projectionEditorWidget->getSelectedEdgeBlendJoint();
    if(_prevJointIndex != -1) {
        _projectionEditorWidget->selectEdgeBlendJoint(_newJointIndex);
    }
}

void SelEdgeBlendJointCmd::undo(){
	ofLogNotice("SelVertexCmd", "undo");
    if(_prevJointIndex != -1) {
        _projectionEditorWidget->selectEdgeBlendJoint(_prevJointIndex);
    }
}

} // namespace piMapper
} // namespace ofx

