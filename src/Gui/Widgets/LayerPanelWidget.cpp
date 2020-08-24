#include "LayerPanelWidget.h"

namespace ofx{
namespace piMapper{

LayerPanelWidget::LayerPanelWidget(){
	_sm = 0;
    rectangle_filled.load("GUI/rectangle-filled.png");
    rectangle_hollow.load("GUI/rectangle-hollow.png");
}

void LayerPanelWidget::draw(){
	if(_sm == 0){
		return;
	}
	
	int numSurfaces = _sm->size();
	
    std::string label = "Layers";
    ofDrawBitmapString(label, ofGetWidth() - 66, 30);

	for(int i = 0; i < numSurfaces; ++i){
		BaseSurface * surface = _sm->getSurface(i);
		BaseSurface * surfaceSelected = _sm->getSelectedSurface();
		
		ofPushStyle();
        ofSetColor(255, 255, 255);
        ofSetRectMode(OF_RECTMODE_CENTER);
		
        int layerIconWidth = 45;
        int layerIconHeight = 20;
        int offsetRight = 20;
        int offsetTop = 50;
        int verticalSpacing = 10;
        int layerIconX = ofGetWidth() - offsetRight - layerIconWidth/2;
        int layerIconY = offsetTop + ((layerIconHeight + verticalSpacing) * (numSurfaces - i - 1));
		
        if(surface == surfaceSelected){
            rectangle_filled.draw(layerIconX,layerIconY);
        }else{
            rectangle_hollow.draw(layerIconX, layerIconY);
        }

//        if(surface == surfaceSelected){
//            ofFill();
//        }else{
//            ofNoFill();
//        }
//        int layerIconWidth = 45;
//		int layerIconHeight = 20;
//		int offsetRight = 20;
//		int offsetTop = 40;
//		int verticalSpacing = 10;
//		int layerIconX = ofGetWidth() - offsetRight - layerIconWidth;
//		int layerIconY = offsetTop + ((layerIconHeight + verticalSpacing) * (numSurfaces - i - 1));

//		ofRectangle layerIcon = ofRectangle(
//			layerIconX,
//			layerIconY,
//			layerIconWidth,
//			layerIconHeight);
        //ofDrawRectangle(layerIcon);


		ofPopStyle();
	}
}

} // namespace piMapper
} // namespace ofx
