#include "RadioList.h"

namespace ofx {
namespace piMapper {

RadioList::RadioList(){
	storedTitle = "";
	storedSelectedItem = 0;
    defaultWidth = 240;
    defaultHeight = 18;
    defaultSpacing = 1;
    b.x = 0;
    b.y = 0;
    b.width = defaultWidth;
    b.height = 0;
    square_hollow.load("GUI/circle-hollow.png");
    square_filled.load("GUI/circle-filled.png");
}

RadioList::~RadioList(){
	clear();
}

void RadioList::setup(std::string title, std::vector<std::string> & labels, std::vector<std::string> & values){

    // Store for later use
	storedTitle = title;
    storedLabels = labels;
    storedValues = values;

    b.height = 0;

    // Create toggles with labels from the labels arg
    for(unsigned int i = 0; i < labels.size(); i++)
    {
        RadioButton * toggle = new RadioButton();
        toggle->setup(labels[i],false,defaultWidth, defaultHeight);
        toggle->setIcons(&square_filled,&square_hollow);
        toggle->setPosition(b.x, b.y + b.height  + defaultSpacing);
        b.height += toggle->getHeight() + defaultSpacing;
        toggle->addListener(this, &RadioList::onToggleClicked);
        radioList.push_back(toggle);
//		#if OF_VERSION_MAJOR == 0 && (OF_VERSION_MINOR >= 8 && OF_VERSION_PATCH >= 2) || (OF_VERSION_MINOR >= 9 && OF_VERSION_PATCH >= 0)
//			toggle->registerMouseEvents();
//		#endif
    }

}

void RadioList::draw(){
    for(unsigned int i = 0; i < radioList.size(); i++)
    {
        radioList.at(i)->draw();
    }
}

void RadioList::setPosition(ofPoint p){

    auto diff = p - b.getPosition();

    for(std::size_t i = 0; i < radioList.size(); i++){
        radioList[i]->setPosition(radioList[i]->getPosition() + diff);
    }

    b.setPosition(p);

}

void RadioList::setInitialPosition(float x, float y){
    b.x = x;
    b.y = y;
}

void RadioList::selectItem(unsigned int index){
    if(index >= radioList.size()){
        return;
    }

	unselectAll();

    radioList[index]->removeListener(this, &RadioList::onToggleClicked);
    *(radioList[index]) = true;          // Select the specific radio button
    radioList[index]->addListener(this, &RadioList::onToggleClicked);

	// Throw event with value that is image path instead of name
	std::string value = storedValues[index];
	ofNotifyEvent(onRadioSelected, value, this);
	storedSelectedItem = index;
}

bool RadioList::selectItemByValue(std::string itemValue){
	if(itemValue == ""){
		ofLogNotice("RadioList") << "Item value empty";
		return false;
	}
	unselectAll();

    for(unsigned int i = 0; i < storedValues.size(); i++){
		if(itemValue == storedValues[i]){
            radioList[i]->removeListener(this, &RadioList::onToggleClicked);
            *(radioList[i]) = true;          // Select the specific radio button
            radioList[i]->addListener(this, &RadioList::onToggleClicked);
            return true;
		}
	}

    ofLogWarning("RadioList") << "Item with value " << itemValue << " not found";
	return false;
}

void RadioList::enable(){
    if(radioList.size() > 0){
        clear();
    }

	// Rebuild everyting
	setup(storedTitle, storedLabels, storedValues);

	// Select the stored selected item without throwing an event
    radioList[storedSelectedItem]->removeListener(this, &RadioList::onToggleClicked);
    *(radioList[storedSelectedItem]) = true;
    radioList[storedSelectedItem]->addListener(this, &RadioList::onToggleClicked);

}

void RadioList::disable(){
	// Just remove everything
	clear();
}

void RadioList::clear(){
    for(unsigned int i = 0; i < radioList.size(); i++) {
        radioList[i]->removeListener(this, &RadioList::onToggleClicked);
        delete radioList[i];
    }
    radioList.clear();
}

void RadioList::unselectAll(){

    for(unsigned int i = 0; i < radioList.size(); i++) {
        radioList[i]->removeListener(this, &RadioList::onToggleClicked);
        *(radioList[i]) = false;
        radioList[i]->addListener(this, &RadioList::onToggleClicked);
    }
}

ofPoint RadioList::getPosition(){

    return ofPoint(b.x,b.y);
}

int RadioList::size(){
    return storedValues.size();
}

void RadioList::onToggleClicked(bool & toggleValue){
	unselectAll();

	// Search for the actual toggle triggering the event
    for(unsigned int i = 0; i < radioList.size(); i++) {
        ofParameter <bool> * paramPtr = static_cast <ofParameter <bool> *>(&radioList[i]->getParameter());

        if(&(paramPtr->get()) == &toggleValue){
            selectItem(i);
            break;
        }
    }
}

} // namespace piMapper
} // namespace ofx
