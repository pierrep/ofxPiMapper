#include "PresentationState.h"

namespace ofx {
namespace piMapper {

PresentationState * PresentationState::_instance = 0;

PresentationState * PresentationState::instance(){
	if(_instance == 0){
		_instance = new ofx::piMapper::PresentationState();
	}
	return _instance;
}

void PresentationState::draw(Application * app){
	app->getGui()->draw();
}

void PresentationState::onMousePressed(Application * app, ofMouseEventArgs & args){
    app->getCmdManager()->exec(
        new ofx::piMapper::SetApplicationStateCmd(
            app, ProjectionMappingState::instance(),
            app->getGui(), GuiMode::PROJECTION_MAPPING));
}

void PresentationState::onKeyPressed(Application * app, ofKeyEventArgs & args)
{
    if(app->getSurfaceManager()->getSelectedSurface() == NULL) {
        app->getSurfaceManager()->selectSurface(0);
    }

    switch(args.key)
    {

    case ' ':
        if(app->getSurfaceManager()->getSelectedSurface()->getSource()->getType() ==
           SourceType::SOURCE_TYPE_VIDEO){
           app->getCmdManager()->exec(
               new ToggleAnimatedSourceCmd(
                   app->getSurfaceManager()->getSelectedSurface()));
        }
        break;

    case ',':
        if(app->getSurfaceManager()->getSelectedSurface()->getSource()->getType() ==
           SourceType::SOURCE_TYPE_VIDEO){
           app->getCmdManager()->exec(
               new StopAnimatedSourceCmd(
                   app->getSurfaceManager()->getSelectedSurface()));
        }
        break;

    case '.':
        if(app->getSurfaceManager()->getSelectedSurface()->getSource()->getType() ==
           SourceType::SOURCE_TYPE_VIDEO){
           app->getCmdManager()->exec(
               new PlayAnimatedSourceCmd(
                   app->getSurfaceManager()->getSelectedSurface()));
        }
        break;

    default:
        break;
    }
}

} // namespace piMapper
} // namespace ofx
