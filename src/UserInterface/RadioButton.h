#pragma once
#include "ofMain.h"

namespace ofx {
namespace piMapper {

class RadioButton {
	public:
		RadioButton();
		~RadioButton();

        void setup();
        void setName(string name);
        bool mouseMoved(ofMouseEventArgs & args);
        bool mousePressed(ofMouseEventArgs & args);
        bool mouseDragged(ofMouseEventArgs & args);
        bool mouseReleased(ofMouseEventArgs & args);
        bool setValue(float mx, float my, bool bCheck);
        void draw();

    protected:

        bool isGuiDrawing();

        bool value;
        string name;
        ofEvent<int> toggleClickedEvent;
        bool bGuiActive;
        ofRectangle b;
        ofRectangle checkboxRect;

    private:
        unsigned long currentFrame;

};

} // namespace piMapper
} // namespace ofx
