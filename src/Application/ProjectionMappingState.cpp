#include "ProjectionMappingState.h"

namespace ofx {
    namespace piMapper {
        
        ProjectionMappingState * ProjectionMappingState::_instance = 0;
    
        ProjectionMappingState * ProjectionMappingState::instance() {
            if (_instance == 0) {
                _instance = new ofx::piMapper::ProjectionMappingState();
            }
            return _instance;
        }
        
        void ProjectionMappingState::draw(Application * app) {}
        
        void ProjectionMappingState::onKeyPressed(Application * app, ofKeyEventArgs & args) {
            switch (args.key) {
                
                case 't':
                    app->getOfxPiMapper()->getCmdManager().exec(
                        new AddSurfaceCmd(
                            app->getOfxPiMapper(),
                            SurfaceType::TRIANGLE_SURFACE)
                    );
                    break;
                    
                case 'q':
                    app->getOfxPiMapper()->getCmdManager().exec(
                        new AddSurfaceCmd(
                            app->getOfxPiMapper(),
                            SurfaceType::QUAD_SURFACE)
                    );
                    break;
                    
                case OF_KEY_BACKSPACE:
                    app->getOfxPiMapper()->getCmdManager().exec(
                        new RmSurfaceCmd(app->getOfxPiMapper()));
                    break;
                
                default:
                    break;
            }
        }
        
        void ProjectionMappingState::onOscMessage(Application * app, ofxOscMessage & oscMessage){
            if (oscMessage.getAddress() == "/moveSelected/x"){
                ofVec2f distance = ofVec2f(0.0f, 0.0f);
                if (oscMessage.getNumArgs()) {
                    float x = oscMessage.getArgAsFloat(0);
                    distance = ofVec2f(x * 10.0f, 0.0f);
                }
                app->getOfxPiMapper()->getProjectionEditor()->moveSelectedSurface(distance);
            } else if (oscMessage.getAddress() == "/moveSelected/y"){
                ofVec2f distance = ofVec2f(0.0f, 0.0f);
                if (oscMessage.getNumArgs()) {
                    float y = oscMessage.getArgAsFloat(0);
                    distance = ofVec2f(0.0f, y * 10.0f);
                }
                app->getOfxPiMapper()->getProjectionEditor()->moveSelectedSurface(distance);
            } else if (oscMessage.getAddress() == "/moveSelectedSurface/") {
                ofVec2f distance = ofVec2f(0.0f, 0.0f);
                if (oscMessage.getNumArgs()) {
                    float x = oscMessage.getArgAsFloat(0);
                    float y = oscMessage.getArgAsFloat(1);
                    distance = ofVec2f(x * 10.0f, y * 10.0f);
                }
                app->getOfxPiMapper()->getProjectionEditor()->moveSelectedSurface(distance);
            } else if (oscMessage.getAddress() == "/selectNext/"){
            
                int selectableSurfaceIndex = 0;
            
                // Check if there is a surface selected
                if (app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) {
                    // If yes, select next surface
                    if (app->getOfxPiMapper()->getSurfaceManager().getSurfaceIndex(
                        app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) ==
                        app->getOfxPiMapper()->getSurfaceManager().size() - 1) {
                        // If last surface is selected
                        // Jump to first surface
                        selectableSurfaceIndex = 0;
                    } else {
                        // Else select next surface
                        selectableSurfaceIndex =
                            app->getOfxPiMapper()->getSurfaceManager().getSurfaceIndex(
                                app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) + 1;
                    }
                }
                
                app->getOfxPiMapper()->getCmdManager().exec(new SelSurfaceCmd(
                    &app->getOfxPiMapper()->getSurfaceManager(),
                    app->getOfxPiMapper()->getSurfaceManager().getSurface(selectableSurfaceIndex),
                    app->getOfxPiMapper()->getProjectionEditor()));
                
            } else if (oscMessage.getAddress() == "/selectPrev/"){
                
                int selectableSurfaceIndex = 0;
            
                // Check if there is a surface selected
                if (app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) {
                    // If yes, select previous surface
                    if (app->getOfxPiMapper()->getSurfaceManager().getSurfaceIndex(
                        app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) == 0) {
                        // If first surface is selected
                        // Jump to last surface
                        selectableSurfaceIndex = app->getOfxPiMapper()->getSurfaceManager().size() - 1;
                    } else {
                        // Else select previous surface
                        selectableSurfaceIndex =
                            app->getOfxPiMapper()->getSurfaceManager().getSurfaceIndex(
                                app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) - 1;
                    }
                }
                
                app->getOfxPiMapper()->getCmdManager().exec(new SelSurfaceCmd(
                    &app->getOfxPiMapper()->getSurfaceManager(),
                    app->getOfxPiMapper()->getSurfaceManager().getSurface(selectableSurfaceIndex),
                    app->getOfxPiMapper()->getProjectionEditor()));
                
            }
        }
    }
}