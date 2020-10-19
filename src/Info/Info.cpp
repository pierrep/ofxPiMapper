#include "Info.h"

namespace ofx {
namespace piMapper {

Info::Info(){
    _show_instructions = false;
    _show_fps = false;
    _text = "There are 4 modes:\n\n"
		" 1. Presentation mode\n"
		" 2. Texture mapping mode\n"
		"  - Press \"<\" and \">\" to select previous or next vertex\n"
		"  - Press <SPACE> to play/pause the video\n"
		" 3. Projection mapping mode\n"
		"  - Press <,> and <.> to select previous or next surface\n"
		"  - Press \"<\" and \">\" to select previous or next vertex\n"
		"  - Press <t> to add new Triangle surface\n"
		"  - Press <q> to add new Quad surface\n"
		"  - Press <r> to add a new ciRcle surface\n"
		"  - Press <p> to toggle Perspective warping while quad surface selected\n"
		"  - Press <g> to add new Grid surface\n"
		"  - Press <[> and <]> to remove or add columns to selected grid surface\n"
		"  - Press <{> and <}> to remove or add rows to selected grid surface\n"
		"  - Press <+> and <-> to scale surface up and down\n"
		"  - Press <9> and <0> to move selected surface one layer up or down\n"
		"  - Press <l> to hide/show Layer panel\n"
		"  - Press <BACKSPACE> to delete selection\n"
		"  - Press <SPACE> to play/pause the video\n"
		"  - Type  <new> to clear composition\n"
		"  - Type  <ext> to exit to command line interface\n"
		"  - Type  <rbt> to reboot\n"
		"  - Type  <sdn> to shutdown\n"
		" 4. Source selection mode\n\n"
		
		"You can switch between the modes by using <1>, <2>, <3> and <4> "
		"keys on the keyboard.\n\n"
		
		"Press <s> to save the composition\n"
		"Press <z> to undo\n"
		"Press <i> to hide this message";
}

void Info::draw(){
    if(_show_instructions){
        ofDrawBitmapStringHighlight(_text, 20, 20,
									ofColor(0, 0, 0, 100),
									ofColor(255, 255, 255, 200));
	}
    if(_show_fps){
            string fps = "FPS: "+ofToString(ofGetFrameRate());
            ofDrawBitmapStringHighlight(fps, 20, ofGetHeight() - 20,
                                        ofColor(0, 0, 0, 100),
                                        ofColor(255, 255, 255, 200));
    }
}

void Info::toggle_instructions(){
    _show_instructions = !_show_instructions;
}

void Info::toggle_fps(){
    _show_fps = !_show_fps;
}

void Info::setText(std::string text){
	_text = text;
}

} // namespace piMapper
} // namespace ofx
