#pragma once

#include "ofMain.h"
//#include "BaseJoint.h"
#include "CircleJoint.h"
#include "Vec2.h"

namespace ofx {
namespace piMapper {
class EdgeBlendJoint : public CircleJoint {

	public:
        EdgeBlendJoint();

        //void update();
        //void draw();
        //bool hitTest(Vec2 position);
        void mouseDragged(ofMouseEventArgs & args);
        const Vec2 getDragPosition();
        void setDragPosition(Vec2 pos);

	private:
        //float radius;
        Vec2 dragPosition;

        //void setDefaultProperties();
        void setDefaultColors();
};

} // namespace piMapper
} // namespace ofx
