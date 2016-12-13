// StopAnimatedSourceCmd
// Play animated (video, gif) sources


#pragma once

#include "BaseCmd.h"
#include "BaseSurface.h"
#include "SourceType.h"
#include "VideoSource.h"

namespace ofx {
namespace piMapper {

class StopAnimatedSourceCmd : public BaseUndoCmd {

	public:
		StopAnimatedSourceCmd(BaseSurface * surface);
		void exec();
		void undo();

	private:
        BaseSurface * _surface;
    
};

} // namespace piMapper
} // namespace ofx

