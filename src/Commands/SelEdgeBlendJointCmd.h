#pragma once

// Created by Pierre Proske 09-04-2020

#include "BaseCmd.h"
#include "SurfaceManager.h"
#include "ProjectionEditorWidget.h"

class ofxPiMapper;

namespace ofx {
namespace piMapper {

class SelEdgeBlendJointCmd : public BaseUndoCmd {

	public:
        SelEdgeBlendJointCmd(ProjectionEditorWidget * p, int i);
		void exec();
		void undo();

	private:
        ProjectionEditorWidget * _projectionEditorWidget;
        int _newJointIndex;
        int _prevJointIndex;

};

} // namespace piMapper
} // namespace ofx

