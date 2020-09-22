#pragma once

#ifdef TARGET_RASPBERRY_PI

#include "ofMain.h"

class ofxOMXPlayer;
class ofxVideoSyncReceiver;

namespace ofx {
namespace piMapper {

class OMXPlayerCache {

	public:
		static OMXPlayerCache * instance();
		ofxVideoSyncReceiver * load(std::string moviePath);
		void unload(std::string moviePath);

	private:
		static OMXPlayerCache * _instance;
		map <std::string, ofxVideoSyncReceiver *> _players;

};

} // namespace piMapper
} // namespace ofx

#endif
