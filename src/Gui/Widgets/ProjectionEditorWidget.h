#pragma once

#include "SurfaceManager.h"
#include "CircleJoint.h"
#include "Vec2.h"
#include "Vec3.h"

namespace ofx {
namespace piMapper {

class ProjectionEditorWidget {
	
    public:
		ProjectionEditorWidget();
	
		void update();
		void draw();
	
		void mouseDragged(ofMouseEventArgs & args);
		void gotMessage(ofMessage & msg);
	
		void setSurfaceManager(SurfaceManager * newSurfaceManager);
		void clearJoints();
		void createJoints();
		void updateJoints();
		void unselectAllJoints();
		void moveSelectedSurface(Vec3 by);
		void stopDragJoints();
		void updateVertices();
		void setSnapDistance(float newSnapDistance);
		CircleJoint * hitTestJoints(Vec2 pos);
        CircleJoint * hitTestEdgeBlendJoints(Vec2 pos);
		std::vector<CircleJoint *> * getJoints();
        const bool getEdgeBlendMode();
        void toggleEdgeBlend();
        void clearEdgeBlendJoints();
        void createEdgeBlendJoints();
        void updateEdgeBlendJoints();
        std::vector<CircleJoint *> * getEdgeBlendJoints();
	
		void onVertexChanged(int & i);
		void onVerticesChanged(std::vector<Vec3> & vertices);
		void onSurfaceSelected(int & surfaceIndex);
		void onVertexSelected(int & vertexIndex);
		void onVertexUnselected(int & vertexIndex);

	private:
		SurfaceManager * surfaceManager;
		std::vector<CircleJoint *> joints;
        std::vector<CircleJoint *> edgeBlendJoints;
		bool bShiftKeyDown;
		float fSnapDistance;
        bool bEdgeBlendMode;

		void drawJoints();
        void drawEdgeBlendJoints();

};

} // namespace piMapper
} // namespace ofx
