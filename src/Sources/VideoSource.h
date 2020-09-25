#pragma once

#include "ofMain.h"
#include "BaseSource.h"
#include "ofxVideoSync.h"

#ifdef TARGET_RASPBERRY_PI
 #include "ofxOMXPlayer.h"
 #include "OMXPlayerCache.h"
#endif

namespace ofx {
namespace piMapper {

class VideoSource : public BaseSource {

	public:

		// TODO: Create enableAudio() and disableAudio() methods
		//       for live audio enabling and disabling.
		static bool enableAudio;
		static bool useHDMIForAudio;

		VideoSource();

		std::string & getPath();
		void loadVideo(std::string & path);
		void setLoop(bool loop);
		bool getLoop();
		void clear();
        void togglePause();
        void stop();
		void update(ofEventArgs & args);

	private:

		#ifdef TARGET_RASPBERRY_PI
			ofxVideoSync * _omxPlayer;
		#else
            unique_ptr<ofxVideoSync> _videoPlayer;
			bool _initialVolumeSet;
		#endif
	
		bool _loop;

};

} // namespace piMapper
} // namespace ofx
