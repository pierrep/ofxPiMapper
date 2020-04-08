// MvSurfaceEdgeBlendJointCmd
// Provides the option to undo moving edge blend joints
// Created by Pierre Proske 09-04-2020

#pragma once

#include "BaseCmd.h"
#include "ProjectionEditorWidget.h"
#include "Vec3.h"

namespace ofx {
namespace piMapper {

class MvSurfaceEdgeBlendJointCmd : public BaseUndoCmd {

	public:
        MvSurfaceEdgeBlendJointCmd(int jointIndex, ProjectionEditorWidget * p);
		void exec();
		void undo();

	private:
        int _jointIndex;
        Vec2 _prevJointPos;
        ProjectionEditorWidget * _projectionEditorWidget;

};

} // namespace piMapper
} // namespace ofx

