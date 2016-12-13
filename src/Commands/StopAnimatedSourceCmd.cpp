#include "StopAnimatedSourceCmd.h"

namespace ofx {
namespace piMapper {

StopAnimatedSourceCmd::StopAnimatedSourceCmd(BaseSurface * surface){
    _surface = surface;
}

void StopAnimatedSourceCmd::exec(){
	ofLogNotice("StopAnimatedSourceCmd", "exec");
    _surface->getSource()->stop();
}

void StopAnimatedSourceCmd::undo(){
	ofLogNotice("StopAnimatedSourceCmd", "undo");
	_surface->getSource()->stop();
}

} // namespace piMapper
} // namespace ofx

