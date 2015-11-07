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
                
            } else if (oscMessage.getAddress() == "/moveSelected/y"){
                
            } else if (oscMessage.getAddress() == "/selectNext/"){
            
                // Check if there is a surface selected
                if (app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) {
                    // If yes, select next surface
                    if (app->getOfxPiMapper()->getSurfaceManager().getSurfaceIndex(
                        app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) ==
                        app->getOfxPiMapper()->getSurfaceManager().size() - 1) {
                        // If last surface is selected
                        // Jump to first surface
                        app->getOfxPiMapper()->getSurfaceManager().selectSurface(0);
                    } else {
                        // Else select next surface
                        app->getOfxPiMapper()->getSurfaceManager().selectSurface(
                            app->getOfxPiMapper()->getSurfaceManager().getSurfaceIndex(
                                app->getOfxPiMapper()->getSurfaceManager().getSelectedSurface()) + 1);
                    }
                } else {
                    // If not, select first surface
                    if (app->getOfxPiMapper()->getSurfaceManager().size()) {
                        app->getOfxPiMapper()->getSurfaceManager().selectSurface(0);
                    }
                }
                
                /*
                if (surfaceManager->getSelectedSurface() != surfaceManager->getSurface(i)){
                    _cmdManager->exec(new SelSurfaceCmd(
                        surfaceManager,
                        surfaceManager->getSurface(i),
                        &projectionEditor));
                }
                */
            } else if (oscMessage.getAddress() == "/selectPrev/"){
                
            }
        }
    }
}