#include "OMXPlayerCache.h"

#ifdef TARGET_RASPBERRY_PI

#include "ofxOMXPlayer.h"
#include "ofxVideoSync.h"
#include "VideoSource.h"

namespace ofx {
namespace piMapper {

OMXPlayerCache * OMXPlayerCache::_instance = 0;

OMXPlayerCache * OMXPlayerCache::instance(){
	if(_instance == 0){
		_instance = new ofx::piMapper::OMXPlayerCache();
	}
	return _instance;
}

ofxVideoSync * OMXPlayerCache::load(std::string moviePath){
	if(_players.find(moviePath) == _players.end()){
		ofxOMXPlayerSettings settings;
		settings.videoPath = moviePath;
		settings.useHDMIForAudio = VideoSource::useHDMIForAudio;
		settings.enableTexture = true;
		settings.enableLooping = true;
		settings.enableAudio = VideoSource::enableAudio;
		
        ofxVideoSync * p = new ofxVideoSync();
        p->setPlayerSettings(settings);
		p->load(moviePath);
		_players[moviePath] = p;		
		
		return p;
	}
	
	_players[moviePath]->getVideoPlayerPtr()->restartMovie();
	return _players[moviePath];
}

void OMXPlayerCache::unload(std::string moviePath){
	if(_players.find(moviePath) != _players.end()){
		_players[moviePath]->setPaused(true);
	}
}

} // namespace piMapper
} // namespace ofx

#endif
