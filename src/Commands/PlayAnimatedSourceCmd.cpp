#include "PlayAnimatedSourceCmd.h"

namespace ofx {
namespace piMapper {

PlayAnimatedSourceCmd::PlayAnimatedSourceCmd(BaseSurface * surface){
    _surface = surface;
}

void PlayAnimatedSourceCmd::exec(){
	ofLogNotice("PlayAnimatedSourceCmd", "exec");
    _surface->getSource()->play();
}

void PlayAnimatedSourceCmd::undo(){
	ofLogNotice("PlayAnimatedSourceCmd", "undo");
	_surface->getSource()->play();
}

} // namespace piMapper
} // namespace ofx

