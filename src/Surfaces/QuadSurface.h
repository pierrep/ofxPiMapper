#pragma once

#include "ofMain.h"
#include "BaseSurface.h"
#include "SurfaceType.h"
#include "HomographyHelper.h"
#include "Vec2.h"
#include "Vec3.h"

namespace ofx {
namespace piMapper {

class QuadSurface : public BaseSurface {
	public:
		QuadSurface();
		~QuadSurface();

		void setup();
		void setup(
			Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4,
			Vec2 t1, Vec2 t2, Vec2 t3, Vec2 t4,
			BaseSource * newSource);
		void draw();
		void setVertex(int index, Vec3 p);
		void setVertices(std::vector<Vec3> v);
		void setTexCoord(int index, Vec2 t);
		void setTexCoords(std::vector<Vec2> t);
		void moveBy(Vec3 v);
        void fullScreen();

		int getType();
		bool hitTest(Vec2 p);
		Vec3 getVertex(int index);
		Vec2 getTexCoord(int index);
		ofPolyline getHitArea();
		ofPolyline getTextureHitArea();
		std::vector<Vec3> getVertices();
		std::vector<Vec2> getTexCoords();

        void setEdges(ofVec4f _edges);
        ofVec4f getEdges();
	
		void setPerspectiveWarping(bool b);
		bool getPerspectiveWarping();
        void setEdgeBlending(bool b);
        bool getEdgeBlending();
	
		ofRectangle getMeshBoundingBox();
		BaseSurface * clone();

	private:
		void calculateHomography();
        void setupShaders();
        void setupVertexArray();
        void updateVertexBuffer();
        void calculateQ();

		float _matrix[16];
		bool _perspectiveWarping;
        bool _edgeBlending;
		ofMesh _meshCache;
        ofShader shader;
        string glESVertexShader;
        string glESFragmentShader;
        string gl3VertexShader;
        string gl3FragmentShader;
        string gl2FragmentShader;
        ofVec4f edges;     

        GLuint VBO, VAO;
        float q0, q1, q2, q3;
        bool _texcoordsChanged;

#ifdef TARGET_OPENGLES
        GLint v3PosAttributeIndex;
        GLint v3TexAttributeIndex;
#endif

};

} // namespace piMapper
} // namespace ofx
