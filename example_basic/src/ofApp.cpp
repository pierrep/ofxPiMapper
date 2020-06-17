#include "ofApp.h"

void ofApp::setup(){
	ofBackground(0);
	mapper.setup();

    #ifdef TARGET_RASPBERRY_PI
        ofSetFullscreen(true);
    #endif
}

void ofApp::update(){
    mapper.update();
}

void ofApp::draw(){    
	mapper.draw();

    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, ofGetHeight()-20);
}

void ofApp::keyPressed(int key){
	mapper.keyPressed(key);
}

void ofApp::keyReleased(int key){
	mapper.keyReleased(key);
}

void ofApp::mouseDragged(int x, int y, int button){
	mapper.mouseDragged(x, y, button);
}

void ofApp::mousePressed(int x, int y, int button){
	mapper.mousePressed(x, y, button);
}

void ofApp::mouseReleased(int x, int y, int button){
	mapper.mouseReleased(x, y, button);
}
