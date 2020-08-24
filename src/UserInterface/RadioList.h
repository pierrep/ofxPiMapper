#pragma once

#include "ofMain.h"
#include "RadioButton.h"

namespace ofx {
namespace piMapper {

class RadioList {
	public:
		RadioList();
		~RadioList();

		void setup(std::string title, std::vector<std::string> & labels, std::vector<std::string> & values);
		void draw();
		void setPosition(ofPoint p);
        void setInitialPosition(float x, float y);
        void selectItem(unsigned int index);
		bool selectItemByValue(std::string itemValue);
		void enable();
		void disable();
		void clear();
		void unselectAll();
		ofPoint getPosition();
        int size();

		// This event notifies about a toggle being selected and passes it's name to
		// the listeners.
		// Use ofAddListener(RadioListInstance.radioSelectedEvent, listenerClassPtr,
		// &listenerClass::listenerMethod)
        // to listen to this. Listener method void listenerMethod(std::string & radioName)
		ofEvent <std::string> onRadioSelected;

	private:
        std::vector<RadioButton *> radioList;
		std::vector<std::string> storedLabels;
		std::vector<std::string> storedValues;
		std::string storedTitle;
        unsigned int storedSelectedItem;
        int defaultWidth;
        int defaultHeight;
        int defaultSpacing;
        ofRectangle b;

		void onToggleClicked(bool & toggleValue);

};

} // namespace piMapper
} // namespace ofx
