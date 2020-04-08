#include "MvSurfaceEdgeBlendJointCmd.h"

namespace ofx {
namespace piMapper {

MvSurfaceEdgeBlendJointCmd::MvSurfaceEdgeBlendJointCmd(int jointIndex, ProjectionEditorWidget * p){
    _jointIndex = jointIndex;
    _projectionEditorWidget = p;
}

void MvSurfaceEdgeBlendJointCmd::exec(){
    ofLogNotice("MvSurfaceEdgeBlendJointCmd", "exec");
    _prevJointPos = _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->getDragPosition();
}

void MvSurfaceEdgeBlendJointCmd::undo(){
	ofLogNotice("MvSurfaceVertCommand", "undo");
    _projectionEditorWidget->getEdgeBlendJoints()->at(_jointIndex)->position = _prevJointPos;
}

} // namespace piMapper
} // namespace ofx

