#include "SetApplicationModeCmd.h"

namespace ofx {
namespace piMapper {

SetApplicationModeCmd::SetApplicationModeCmd(
	Application * app,
	ApplicationBaseMode * st){

	_application = app;
	_prevApplicationState = 0;
	_applicationState = st;
}

void SetApplicationModeCmd::exec(){
	_prevApplicationState = _application->getState();
	_application->setState(_applicationState);
	_applicationState->setTranslation(ofPoint(0, 0));
	_translation = _prevApplicationState->getTranslation();
	
	Gui::instance()->getTextureEditorWidget().setSurface(
		_application->getSurfaceManager()->getSelectedSurface());
	Gui::instance()->getTextureHighlightWidget().findConsumerSurfaces();
	
	if(_applicationState != PresentationMode::instance()){
		ofShowCursor();
		ofSetVerticalSync(true);
        for(unsigned int i = 0; i <_application->getSurfaceManager()->getActivePreset()->getSurfaces().size();i++) {
            if(_application->getSurfaceManager()->getActivePreset()->getSurfaces().at(i)->getSource()->getType() == SourceType::SOURCE_TYPE_VIDEO)
            {
                VideoSource* video = dynamic_cast<VideoSource*>(_application->getSurfaceManager()->getActivePreset()->getSurfaces().at(i)->getSource());
                video->disableVideoSync();
                ofLogNotice() << "Not Presentation mode, don't use video sync";
            }
        }
	}else{
		ofHideCursor();
		ofSetVerticalSync(false);
		ofSetFrameRate(30);
        for(unsigned int i = 0; i <_application->getSurfaceManager()->getActivePreset()->getSurfaces().size();i++) {
            if(_application->getSurfaceManager()->getActivePreset()->getSurfaces().at(i)->getSource()->getType() == SourceType::SOURCE_TYPE_VIDEO)
            {
                VideoSource* video = dynamic_cast<VideoSource*>(_application->getSurfaceManager()->getActivePreset()->getSurfaces().at(i)->getSource());
                video->enableVideoSync();
                ofLogNotice() << "Presentation mode, use video sync";
            }
        }
	}
	
	if(_applicationState == SourceSelectionMode::instance()){
		Gui::instance()->getSourcesEditorWidget().enable();
	}else{
		Gui::instance()->getSourcesEditorWidget().disable();
	}
}

void SetApplicationModeCmd::undo(){
	ofLogNotice("SetApplicationModeCmd", "undo");
	_application->setState(_prevApplicationState);
	_application->getState()->setTranslation(_translation);
	
	Gui::instance()->getTextureEditorWidget().setSurface(
		_application->getSurfaceManager()->getSelectedSurface());
	
	if(_prevApplicationState != PresentationMode::instance()){
		ofShowCursor();
		ofSetVerticalSync(true);	}else{
		ofHideCursor();
		ofSetVerticalSync(false);
		ofSetFrameRate(30);
	}
	
	if(_prevApplicationState == SourceSelectionMode::instance()){
		Gui::instance()->getSourcesEditorWidget().enable();
	}else{
		Gui::instance()->getSourcesEditorWidget().disable();
	}
}

} // namespace piMapper
} // namespace ofx

