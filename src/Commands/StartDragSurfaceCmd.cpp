#include "StartDragSurfaceCmd.h"

namespace ofx {
namespace piMapper {

StartDragSurfaceCmd::StartDragSurfaceCmd(BaseSurface * surface){
	_surface = surface;
}

void StartDragSurfaceCmd::exec(){
    ofLogVerbose("StartDragSurfaceCmd", "exec");
	_previousVertices = _surface->getVertices();
	_surface->setMoved(false);
}

void StartDragSurfaceCmd::undo(){
    ofLogVerbose("StartDragSurfaceCmd", "undo");
	Vec3 step = _previousVertices[0] - _surface->getVertices()[0];
	_surface->moveBy(step);
}

} // namespace piMapper
} // namespace ofx

