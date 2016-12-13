#pragma once

#include "Application.h"
#include "ofEvents.h"
#include "ofLog.h"
#include "ofGraphics.h"
#include "SetApplicationStateCmd.h"
#include "ProjectionMappingState.h"
#include "PlayAnimatedSourceCmd.h"
#include "StopAnimatedSourceCmd.h"
#include "ToggleAnimatedSourceCmd.h"
#include "GuiMode.h"

namespace ofx {
namespace piMapper {

class PresentationState : public ApplicationBaseState {

	public:
		static PresentationState * instance();
		void draw(Application * app);
        void onMousePressed(Application * app, ofMouseEventArgs & args);
        void onKeyPressed(Application * app, ofKeyEventArgs & args);

	private:
		static PresentationState * _instance;

};

} // namespace piMapper
} // namespace ofx
