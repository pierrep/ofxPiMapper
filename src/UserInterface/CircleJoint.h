#pragma once

#include "ofMain.h"
#include "BaseJoint.h"
#include "Vec2.h"

namespace ofx {
namespace piMapper {
class CircleJoint : public BaseJoint {

	public:
		CircleJoint();

		void update();
		void draw();
		bool hitTest(Vec2 position);

    protected:
		float radius;

		void setDefaultProperties();
        ofImage circle;

};

} // namespace piMapper
} // namespace ofx
