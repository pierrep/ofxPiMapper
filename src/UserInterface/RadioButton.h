#pragma once
#include "ofMain.h"

namespace ofx {
namespace piMapper {

class RadioButton {
	public:
		RadioButton();
        ~RadioButton();

        void setup(bool value, float width, float height);
        //void setup(const std::string& toggleName, bool value, float width, float height);
        void setName(string name);
        void setPosition(ofPoint p);
        void setPosition(float x, float y);
        ofPoint getPosition() const;
        float getHeight() const;
        bool mouseMoved(ofMouseEventArgs & args);
        bool mousePressed(ofMouseEventArgs & args);
        bool mouseDragged(ofMouseEventArgs & args);
        bool mouseReleased(ofMouseEventArgs & args);
        bool mouseScrolled(ofMouseEventArgs & args){return false;}
        void mouseEntered(ofMouseEventArgs &){}
        void mouseExited(ofMouseEventArgs &){}
        bool setValue(float mx, float my, bool bCheck);
        void draw();

        bool operator=(bool v);
        ofAbstractParameter & getParameter();

        template<class ListenerClass, typename ListenerMethod>
        void addListener(ListenerClass * listener, ListenerMethod method){
            value.addListener(listener,method);
        }

        template<class ListenerClass, typename ListenerMethod>
        void removeListener(ListenerClass * listener, ListenerMethod method){
            value.removeListener(listener,method);
        }


    protected:

        bool isGuiDrawing();

        ofParameter<bool> value;
        //bool value;
        string name;
        ofEvent<int> toggleClickedEvent;
        bool bGuiActive;
        ofRectangle b;
        ofRectangle checkboxRect;
        int defaultTextPadding;

    private:
        unsigned long currentFrame;

};

} // namespace piMapper
} // namespace ofx
