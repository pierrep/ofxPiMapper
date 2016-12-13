// PlayAnimatedSourceCmd
// Play animated (video, gif) sources


#pragma once

#include "BaseCmd.h"
#include "BaseSurface.h"
#include "SourceType.h"
#include "VideoSource.h"

namespace ofx {
namespace piMapper {

class PlayAnimatedSourceCmd : public BaseUndoCmd {

	public:
		PlayAnimatedSourceCmd(BaseSurface * surface);
		void exec();
		void undo();

	private:
        BaseSurface * _surface;
    
};

} // namespace piMapper
} // namespace ofx

