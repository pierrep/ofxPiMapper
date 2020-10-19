#pragma once

#include "ofMain.h"

namespace ofx {
namespace piMapper {

class Info {
	public:
		Info();
	
		void draw();
        void toggle_instructions();
        void toggle_fps();
		void setText(std::string text);
	
	private:
        bool _show_instructions;
        bool _show_fps;
		std::string _text;
};

} // namespace piMapper
} // namespace ofx
