#pragma once

#ifdef TARGET_RASPBERRY_PI

#include "ofMain.h"

class ofxOMXPlayer;
class ofxVideoSync;

namespace ofx {
namespace piMapper {

class OMXPlayerCache {

	public:
		static OMXPlayerCache * instance();
        ofxVideoSync * load(std::string moviePath);
		void unload(std::string moviePath);

	private:
		static OMXPlayerCache * _instance;
        map <std::string, ofxVideoSync *> _players;

};

} // namespace piMapper
} // namespace ofx

#endif
