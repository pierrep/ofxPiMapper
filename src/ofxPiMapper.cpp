#include "ofxPiMapper.h"

ofxPiMapper::ofxPiMapper(){}

void ofxPiMapper::setup(){
	_application.setup();
}

void ofxPiMapper::draw(){
	_application.draw();
}

void ofxPiMapper::play(){
    _application.play();
}

void ofxPiMapper::stop(){
    _application.stop();
}

void ofxPiMapper::registerFboSource(ofx::piMapper::FboSource & fboSource){
	_application.addFboSource(fboSource);
}

bool ofxPiMapper::loadXmlSettings(string fileName){
	return _application.loadXmlSettings(fileName);
}
