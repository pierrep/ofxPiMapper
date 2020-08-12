#include "RadioButton.h"

namespace ofx {
namespace piMapper {

RadioButton::RadioButton(){


}

bool RadioButton::isGuiDrawing(){
    if(ofGetFrameNum() - currentFrame > 1){
        return false;
    }else{
        return true;
    }
}

void RadioButton::setup()
{
    ofRegisterMouseEvents(this);
}

void RadioButton::setName(string _name)
{
    name = _name;
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
//        checkRect.x += b.x;
//        checkRect.y += b.y;

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

}


} // namespace piMapper
} // namespace ofx
