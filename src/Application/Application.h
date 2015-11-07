#pragma once

#include "ofEvents.h"
#include "ofLog.h"

#include "ofxPiMapper.h"
#include "ofxOsc.h"

#include "SetApplicationStateCmd.h"

#include "ApplicationBaseState.h"
#include "PresentationState.h"
#include "ProjectionMappingState.h"
#include "TextureMappingState.h"
#include "SourceSelectionState.h"

// TODO: To be removed.
#include "GuiMode.h"

class ofxPiMapper;

namespace ofx {
    namespace piMapper {
        
        class ApplicationBaseState;
    
        class Application {
            public:
                Application(ofxPiMapper * opm);
                ~Application();
            
                ApplicationBaseState * getState();
                ofxPiMapper * getOfxPiMapper(); // Temporary method.
            
                void update();
                void draw();
                void onKeyPressed(ofKeyEventArgs & args);
                void onOscMessage(ofxOscMessage & oscMessage);
                            
            protected:
                void setState(ApplicationBaseState * st);
            
            private:
                friend class ApplicationBaseState;
                friend class SetApplicationStateCmd;
            
                ApplicationBaseState * _state;
                ofxPiMapper * _ofxPiMapper;
                ofxOscReceiver * _oscReceiver;
        };
        
    } // namespace piMapper
} // namespace ofx
