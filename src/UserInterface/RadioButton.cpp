#include "RadioButton.h"

namespace ofx {
namespace piMapper {

RadioButton::~RadioButton(){
    ofUnregisterMouseEvents(this);
}

RadioButton::RadioButton(){
    defaultTextPadding = 4;

}

bool RadioButton::isGuiDrawing(){
    if(ofGetFrameNum() - currentFrame > 1){
        return false;
    }else{
        return true;
    }
}

void RadioButton::setup(bool _value, float width, float height)
{
    b.x = 0;
    b.y = 0;
    b.width = width;
    b.height = height;
    bGuiActive = false;
    value = _value;
    checkboxRect.set(1, 1, b.height - 2, b.height - 2);
    ofRegisterMouseEvents(this);
    return;
}

void RadioButton::setName(string _name)
{
    name = _name;
}

void RadioButton::setPosition(ofPoint p)
{
    setPosition(p.x, p.y);
}

void RadioButton::setPosition(float x, float y)
{
    b.x = x;
    b.y = y;
}

ofPoint RadioButton::getPosition() const {
    return ofPoint(b.x, b.y, 0);
}

float RadioButton::getHeight() const {
    return b.height;
}

bool RadioButton::mouseMoved(ofMouseEventArgs & args)
{
    if(isGuiDrawing() && b.inside(args)){
        return true;
    }else{
        return false;
    }
}

bool RadioButton::mousePressed(ofMouseEventArgs & args)
{
    if(setValue(args.x, args.y, true)){
        return true;
    }else{
        return false;
    }
}

bool RadioButton::mouseDragged(ofMouseEventArgs & args)
{
    if(bGuiActive && b.inside(args)){
        return true;
    }else{
        return false;
    }
}

bool RadioButton::mouseReleased(ofMouseEventArgs & args)
{
    bool wasGuiActive = bGuiActive;
    bGuiActive = false;
    if(wasGuiActive && b.inside(args)){
        return true;
    }else{
        return false;
    }
}

bool RadioButton::setValue(float mx, float my, bool bCheck){

    if( !isGuiDrawing() ){
        bGuiActive = false;
        return false;
    }

    if( bCheck ){
        ofRectangle checkRect = checkboxRect;
        checkRect.x += b.x;
        checkRect.y += b.y;

        if( checkRect.inside(mx, my) ){
            bGuiActive = true;
        }else{
            bGuiActive = false;

        }
    }
    if( bGuiActive ){
        value = !value;
        return true;
    }
    return false;
}

void RadioButton::draw()
{
    currentFrame = ofGetFrameNum();

    ofPushStyle();
    ofSetColor(255,0,255);
    ofDrawRectangle(b);

    if( value ){
        ofSetColor(0);
    } else {
        ofSetColor(255);
    }
    ofDrawRectangle(b.getPosition()+checkboxRect.getTopLeft(),checkboxRect.width,checkboxRect.height);

    float textX = b.x + defaultTextPadding + checkboxRect.width;
    float textY = b.getCenter().y;
    ofDrawBitmapString(name,textX,textY);

    ofPopStyle();
}

ofAbstractParameter & RadioButton::getParameter(){
    return value;
}

bool RadioButton::operator=(bool v){
    value = v;
    return v;
}



} // namespace piMapper
} // namespace ofx
