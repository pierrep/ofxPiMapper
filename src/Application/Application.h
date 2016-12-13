#pragma once

#include "ofEvents.h"
#include "ofLog.h"

#include "SetApplicationStateCmd.h"

#include "ApplicationBaseState.h"
#include "PresentationState.h"
#include "ProjectionMappingState.h"
#include "TextureMappingState.h"
#include "SourceSelectionState.h"
#include "FboSource.h"
#include "Info.h"

#include "SurfaceManagerGui.h"

// TODO: To be removed.
#include "GuiMode.h"

#include "Gui.h"
#include "TerminalListener.h"

#define PIMAPPER_DEF_SURFACES_XML_FILE "defaultSurfaces.xml"
#define PIMAPPER_USER_SURFACES_XML_FILE "surfaces.xml"

namespace ofx {
namespace piMapper {

class ApplicationBaseState;

class Application : public KeyListener {

	public:
		Application();

		ApplicationBaseState * getState();

		void setup();
		void draw();
        void play();
        void stop();
	
		void onKeyPressed(ofKeyEventArgs & args);
		void onKeyReleased(ofKeyEventArgs & args);
	
		// We use this to pass mouse events into the GUI layer
		void onMousePressed(ofMouseEventArgs & args);
	
		// Then we catch GUI events with this one and create commands
		void onJointPressed(GuiJointEvent & e);
		void onSurfacePressed(GuiSurfaceEvent & e);
		void onBackgroundPressed(GuiBackgroundEvent & e);
		// Every state should have it's own GUI layer
	
		void addFboSource(FboSource & fboSource);
	
		bool loadXmlSettings(string fileName);
		bool isShiftKeyDown();
	
		SurfaceManagerGui * getGui(){ return &_gui; };
		SurfaceManager * getSurfaceManager(){ return &_surfaceManager; };
		CmdManager * getCmdManager(){ return &_cmdManager; };
	
		void onCharacterReceived(KeyListenerEventData & e);
		TerminalListener consoleListener;
	
	protected:
		void setState(ApplicationBaseState * st);
	
	private:
		friend class ApplicationBaseState;
		friend class SetApplicationStateCmd;

		ApplicationBaseState * _state;
	
		CmdManager _cmdManager;
		SurfaceManagerGui _gui;
		MediaServer _mediaServer;
		SurfaceManager _surfaceManager;
		Info _info;
	
		bool _shiftKeyDown;
        bool _isSSHConnection;

};

} // namespace piMapper
} // namespace ofx
