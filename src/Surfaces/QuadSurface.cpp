#include "QuadSurface.h"

namespace ofx {
namespace piMapper {

    QuadSurface::QuadSurface()
    {
        _perspectiveWarping = false;
        _edgeBlendingMode = false;
        _texcoordsChanged = false;
        edges = ofVec4f::zero();
        setup();
    }

    QuadSurface::~QuadSurface()
    {
        std::cout << "QuadSurface destructor." << std::endl;
    }

    void QuadSurface::setup()
    {
        // Create 4 points for the 2 triangles
        Vec3 p1 = Vec3(0.0f, 0.0f, 0.0f);
        Vec3 p2 = Vec3(0.0f, ofGetHeight(), 0.0f);
        Vec3 p3 = Vec3(ofGetWidth(), ofGetHeight(), 0.0f);
        Vec3 p4 = Vec3(ofGetWidth(), 0.0f, 0.0f);

        // Create 4 point for the texture coordinates
        Vec2 t1 = Vec2(0.0f, 0.0f);
        Vec2 t2 = Vec2(1.0f, 0.0f);
        Vec2 t3 = Vec2(1.0f, 1.0f);
        Vec2 t4 = Vec2(0.0f, 1.0f);

        setup(p1, p2, p3, p4, t1, t2, t3, t4, source);
    }

    void QuadSurface::setup(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4,
        Vec2 t1, Vec2 t2, Vec2 t3, Vec2 t4,
        BaseSource* newSource)
    {
        // Assign texture
        source = newSource;

        // Clear mesh
        mesh.clear();

        // Create a surface with the points
        mesh.addVertex(p1.toOf());
        mesh.addVertex(p2.toOf());
        mesh.addVertex(p3.toOf());
        mesh.addVertex(p4.toOf());

        // Add 2 triangles
        mesh.addTriangle(0, 2, 3);
        mesh.addTriangle(0, 1, 2);

        // Add texture coordinates
        mesh.addTexCoord(t1.toOf());
        mesh.addTexCoord(t2.toOf());
        mesh.addTexCoord(t3.toOf());
        mesh.addTexCoord(t4.toOf());

        _meshCache = mesh;

        setupShaders();

        if (ofIsGLProgrammableRenderer()) {
            setupVertexArray();
        } else {
            calculateHomography();
        }
    }

    void QuadSurface::draw()
    {
        if (source->getTexture() == 0) {
            return;
        }

        if (!source->getTexture()->isAllocated()) {
            return;
        }

        if (_perspectiveWarping) {
            bool meshChanged = false;

            if (
                mesh.getVertices()[0] != _meshCache.getVertices()[0] || mesh.getVertices()[1] != _meshCache.getVertices()[1] || mesh.getVertices()[2] != _meshCache.getVertices()[2] || mesh.getVertices()[3] != _meshCache.getVertices()[3]) {
                meshChanged = true;
            }

            if (_texcoordsChanged) {
                meshChanged = true;
                _texcoordsChanged = false;
            }

            //            ofRectangle box = getMeshBoundingBox();
            //            ofMesh m = mesh;

            //            m.setVertex(0, Vec3(0, 0, 0).toOf());
            //            m.setVertex(1, Vec3(box.width, 0, 0).toOf());
            //            m.setVertex(2, Vec3(box.width, box.height, 0).toOf());
            //            m.setVertex(3, Vec3(0, box.height, 0).toOf());

#ifdef TARGET_OPENGLES
			if (ofIsGLProgrammableRenderer()) {
				if (meshChanged) {
					calculateQ();
					updateVertexBuffer();
					_meshCache = mesh;
				}

				ofPushMatrix();
				bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
				ofEnableNormalizedTexCoords();

				shader.begin();
				shader.setUniformTexture("tex", *(source->getTexture()), 0);
				shader.setUniform1i("edgeBlend", _edgeBlendingMode ? 1 : 0);
				shader.setUniform1i("w", 1);
				shader.setUniform1i("h", 1);
				shader.setUniform4f("edges", edges.x, edges.y, edges.z, edges.w);
				shader.setUniform4f("texoffset", getTexCoords()[0].x, getTexCoords()[1].y, getTexCoords()[2].x, getTexCoords()[3].y);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);

				glEnableVertexAttribArray(v3PosAttributeIndex);
				glVertexAttribPointer(v3PosAttributeIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

				glEnableVertexAttribArray(v3TexAttributeIndex);
				glVertexAttribPointer(v3TexAttributeIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));

				glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 0 = the starting index in the enabled arrays; 4 = the number of indices to be rendered.

				glDisableVertexAttribArray(v3PosAttributeIndex);
				glDisableVertexAttribArray(v3TexAttributeIndex);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				shader.end();

				if (!normalizedTexCoords) {
					ofDisableNormalizedTexCoords();
				}

				ofPopMatrix();
            
			} else {
                if (meshChanged) {
                    calculateHomography();
                    _meshCache = mesh;
                }

                ofRectangle box = getMeshBoundingBox();
                ofMesh m = mesh;

                m.setVertex(0, Vec3(0, 0, 0).toOf());
                m.setVertex(1, Vec3(box.width, 0, 0).toOf());
                m.setVertex(2, Vec3(box.width, box.height, 0).toOf());
                m.setVertex(3, Vec3(0, box.height, 0).toOf());

                ofPushMatrix();
                if (true) {
                    bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
                    ofEnableNormalizedTexCoords();

                    glMultMatrixf(_matrix);

                    source->getTexture()->bind();
                    m.draw();
                    source->getTexture()->unbind();
  
                    if (!normalizedTexCoords) {
                        ofDisableNormalizedTexCoords();
                    }
                }
                ofPopMatrix();
            }
#else
            if (ofIsGLProgrammableRenderer()) {

                if (meshChanged) {
                    calculateQ();
                    updateVertexBuffer();
                    _meshCache = mesh;
                }

                ofPushMatrix();
                bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
                ofEnableNormalizedTexCoords();

                shader.begin();
                shader.setUniformTexture("tex", *(source->getTexture()), 0);
                shader.setUniform1i("edgeBlend", _edgeBlendingMode ? true : false);
                shader.setUniform1i("w", 1);
                shader.setUniform1i("h", 1);
                shader.setUniform4f("edges", edges.x, edges.y, edges.z, edges.w);
                shader.setUniform4f("texoffset", getTexCoords()[0].x, getTexCoords()[1].y, getTexCoords()[2].x, getTexCoords()[3].y);

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 0 = the starting index in the enabled arrays; 4 = the number of indices to be rendered.
                glBindVertexArray(0);

                shader.end();

                if (!normalizedTexCoords) {
                    ofDisableNormalizedTexCoords();
                }

                ofPopMatrix();
            } else {
                if (meshChanged) {
                    calculateHomography();
                    _meshCache = mesh;
                }

                ofRectangle box = getMeshBoundingBox();
                ofMesh m = mesh;

                m.setVertex(0, Vec3(0, 0, 0).toOf());
                m.setVertex(1, Vec3(box.width, 0, 0).toOf());
                m.setVertex(2, Vec3(box.width, box.height, 0).toOf());
                m.setVertex(3, Vec3(0, box.height, 0).toOf());

                ofPushMatrix();
                if (true) {
                    bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
                    ofEnableNormalizedTexCoords();

                    glMultMatrixf(_matrix);
                    if (_edgeBlendingMode) {
                        shader.begin();
                        shader.setUniformTexture("tex0", *(source->getTexture()), 0);
                        shader.setUniform1i("w", source->getTexture()->getWidth());
                        shader.setUniform1i("h", source->getTexture()->getHeight());
                        shader.setUniform4f("edges", edges.x, edges.y, edges.z, edges.w);
                        shader.setUniform4f("texoffset", getTexCoords()[0].x, getTexCoords()[1].y, getTexCoords()[2].x, getTexCoords()[3].y);
                    }
                    source->getTexture()->bind();
                    m.draw();
                    source->getTexture()->unbind();
                    if (_edgeBlendingMode) {
                        shader.end();
                    }

                    if (!normalizedTexCoords) {
                        ofDisableNormalizedTexCoords();
                    }
                }
                ofPopMatrix();
            }
#endif
        } else {
            bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
            ofEnableNormalizedTexCoords();

            ofPushStyle();
            ofSetColor(255, 255, 255);

            source->getTexture()->bind();
            mesh.draw();
            source->getTexture()->unbind();

            ofPopStyle();

            if (!normalizedTexCoords) {
                ofDisableNormalizedTexCoords();
            }
        }
    }

    void QuadSurface::setVertex(int index, Vec3 p)
    {
        if (index > 3) {
            ofLog() << "Vertex with this index does not exist: " << index << std::endl;
            return;
        }

        mesh.setVertex(index, p.toOf());
        ofNotifyEvent(vertexChangedEvent, index, this);
    }

    void QuadSurface::setVertices(std::vector<Vec3> v)
    {
        if (v.size() != 4) {
            throw runtime_error("Wrong numexponentber of vertices");
        }

        for (int i = 0; i < 4; ++i) {
            mesh.setVertex(i, v[i].toOf());
        }

        std::vector<Vec3> vertices = Vec3::fromOf(mesh.getVertices());
        ofNotifyEvent(verticesChangedEvent, vertices, this);
    }

    void QuadSurface::setTexCoord(int index, Vec2 t)
    {
        if (index > 3) {
            ofLog() << "Texture coordinate with this index does not exist: " << index
                    << std::endl;
            return;
        }

        mesh.setTexCoord(index, t.toOf());
        if (ofIsGLProgrammableRenderer()) {
            _texcoordsChanged = true;
        }
    }

    void QuadSurface::setTexCoords(std::vector<Vec2> t)
    {
        if (t.size() != 4) {
            throw runtime_error("Wrong number of vertices");
        }
        for (int i = 0; i < 4; ++i) {
            mesh.setTexCoord(i, t[i].toOf());
        }
        if (ofIsGLProgrammableRenderer()) {
            _texcoordsChanged = true;
        }
    }

    void QuadSurface::moveBy(Vec3 v)
    {
        for (int i = 0; i < mesh.getVertices().size(); i++) {
            mesh.getVertices()[i] += v.toOf();
        }

        setMoved(true);
        std::vector<Vec3> vertices = Vec3::fromOf(mesh.getVertices());
        ofNotifyEvent(verticesChangedEvent, vertices, this);
    }
    
	void QuadSurface::fullScreen(){
		mesh.getVertices()[0] = Vec3(0, 0, 0.0f).toOf();
		mesh.getVertices()[1] = Vec3((float)ofGetWidth(), 0, 0.0f).toOf();
		mesh.getVertices()[2] = Vec3((float)ofGetWidth(), (float)ofGetHeight(), 0.0f).toOf();
		mesh.getVertices()[3] = Vec3(0, (float)ofGetHeight(), 0.0f).toOf();

		setMoved(true);
		std::vector<Vec3> vertices = Vec3::fromOf(mesh.getVertices());
		ofNotifyEvent(verticesChangedEvent, vertices, this);
	}    

    int QuadSurface::getType()
    {
        return SurfaceType::QUAD_SURFACE;
    }

    bool QuadSurface::hitTest(Vec2 p)
    {
        ofPolyline line = getHitArea();

        if (line.inside(p.x, p.y)) {
            return true;
        } else {
            return false;
        }
    }

    Vec3 QuadSurface::getVertex(int index)
    {
        if (index > 3) {
            ofLog() << "Vertex with this index does not exist: " << index << std::endl;
            throw runtime_error("Vertex index out of bounds.");
        }

        return Vec3(
            mesh.getVertex(index).x,
            mesh.getVertex(index).y,
            mesh.getVertex(index).z);
    }

    Vec2 QuadSurface::getTexCoord(int index)
    {
        if (index > 3) {            
            throw runtime_error("Texture coordinate index out of bounds.");
        }

        return Vec2(
            mesh.getTexCoord(index).x,
            mesh.getTexCoord(index).y);
    }

    ofPolyline QuadSurface::getHitArea()
    {
        ofPolyline line;

        line.addVertex(ofPoint(mesh.getVertex(0).x, mesh.getVertex(0).y));
        line.addVertex(ofPoint(mesh.getVertex(1).x, mesh.getVertex(1).y));
        line.addVertex(ofPoint(mesh.getVertex(2).x, mesh.getVertex(2).y));
        line.addVertex(ofPoint(mesh.getVertex(3).x, mesh.getVertex(3).y));
        line.close();

        return line;
    }

    ofPolyline QuadSurface::getTextureHitArea()
    {
        ofPolyline line;
        Vec2 textureSize = Vec2(source->getTexture()->getWidth(),
            source->getTexture()->getHeight());
        for (int i = 0; i < mesh.getTexCoords().size(); i++) {
            line.addVertex(ofPoint(mesh.getTexCoords()[i] * textureSize.toOf()));
        }
        line.close();

        return line;
    }

    std::vector<Vec3> QuadSurface::getVertices()
    {
        return Vec3::fromOf(mesh.getVertices());
    }

    std::vector<Vec2> QuadSurface::getTexCoords()
    {
        return Vec2::fromOf(mesh.getTexCoords());
    }

    void QuadSurface::calculateHomography()
    {
        float src[4][2];
        float dst[4][2];

        ofRectangle box = getMeshBoundingBox();

        src[0][0] = 0;
        src[0][1] = 0;
        src[1][0] = box.width;
        src[1][1] = 0;
        src[2][0] = box.width;
        src[2][1] = box.height;
        src[3][0] = 0;
        src[3][1] = box.height;

        Vec3 p0(mesh.getVertex(0).x, mesh.getVertex(0).y, mesh.getVertex(0).z);
        Vec3 p1(mesh.getVertex(1).x, mesh.getVertex(1).y, mesh.getVertex(1).z);
        Vec3 p2(mesh.getVertex(2).x, mesh.getVertex(2).y, mesh.getVertex(2).z);
        Vec3 p3(mesh.getVertex(3).x, mesh.getVertex(3).y, mesh.getVertex(3).z);

        dst[0][0] = p0.x;
        dst[0][1] = p0.y;
        dst[1][0] = p1.x;
        dst[1][1] = p1.y;
        dst[2][0] = p2.x;
        dst[2][1] = p2.y;
        dst[3][0] = p3.x;
        dst[3][1] = p3.y;

        HomographyHelper::findHomography(src, dst, _matrix);
    }

    void QuadSurface::setEdges(ofVec4f _edges)
    {
        edges = _edges;
    }

    void QuadSurface::setPerspectiveWarping(bool b)
    {
        _perspectiveWarping = b;
    }

    bool QuadSurface::getPerspectiveWarping()
    {
        return _perspectiveWarping;
    }

    void QuadSurface::setEdgeBlendMode(bool b)
    {
        _edgeBlendingMode = b;
    }

    ofRectangle QuadSurface::getMeshBoundingBox()
    {
        float minX = 10000.0f;
        float minY = 10000.0f;
        float maxX = 0.0f;
        float maxY = 0.0f;

        for (int i = 0; i < mesh.getVertices().size(); ++i) {
            if (mesh.getVertices()[i].x < minX) {
                minX = mesh.getVertices()[i].x;
            }

            if (mesh.getVertices()[i].y < minY) {
                minY = mesh.getVertices()[i].y;
            }

            if (mesh.getVertices()[i].x > maxX) {
                maxX = mesh.getVertices()[i].x;
            }

            if (mesh.getVertices()[i].y > maxY) {
                maxY = mesh.getVertices()[i].y;
            }
        }

        ofRectangle boundingBox = ofRectangle(ofPoint(minX, minY), ofPoint(maxX, maxY));
        return boundingBox;
    }

    BaseSurface* QuadSurface::clone()
    {
        QuadSurface* s = new QuadSurface();
        s->setVertices(getVertices());
        s->setTexCoords(getTexCoords());
        s->setPerspectiveWarping(getPerspectiveWarping());
        BaseSource* src = getSource();
        src->referenceCount++;
        s->setSource(src);
        return s;
    }

#define STRINGIFY(A) #A

    void QuadSurface::setupShaders()
    {
        glESVertexShader = STRINGIFY(
            precision highp float;

            uniform mat4 modelViewProjectionMatrix;

            attribute vec3 position;
            attribute vec3 texcoord;

            varying vec3 texCoordVarying;

            void main() {
                gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
                texCoordVarying = texcoord;
            });

        glESFragmentShader = STRINGIFY(
            precision highp float;

            uniform sampler2D tex;
            varying vec3 texCoordVarying;

            // used in a few inversions
            const vec3 one = vec3(1.0);

            // controls the interpolation curve ([1..n], 1.0 = linear, 2.0 = default quadratic)
            uniform float exponent; // try: 2.0;
            // controls the center of interpolation ([0..1], 0.5 = linear)
            uniform vec3 luminance; // try: vec3(0.5);
            // controls gamma levels ([1..n], 1.8 or 2.2 is typical)
            uniform vec3 gamma; // try: vec3(1.8, 1.5, 1.2);
            // controls blending area at left, top, right and bottom in percentages ([0..0.5])
            uniform vec4 edges; // try: vec4(0.4, 0.4, 0.0, 0.0);
            uniform vec4 texoffset;
            uniform int w;
            uniform int h;
            uniform bool edgeBlend;

            void main() {
                vec2 uv = texCoordVarying.xy / texCoordVarying.z;
                vec4 col = texture2D(tex, uv);

                vec3 blend = vec3(1.0, 1.0, 1.0);

                // calculate edge blending factor
                if (edgeBlend) {
                    float a = 1.0;
                    if (edges.x > 0.0)
                        a *= clamp((uv.x / float(w) - texoffset.x) / edges.x, 0.0, 1.0);
                    if (edges.y > 0.0)
                        a *= clamp((uv.y / float(h) - texoffset.y) / edges.y, 0.0, 1.0);
                    if (edges.z > 0.0)
                        a *= clamp((1.0 - (uv.x / float(w)) - (1.0 - texoffset.z)) / edges.z, 0.0, 1.0);
                    if (edges.w > 0.0)
                        a *= clamp((1.0 - (uv.y / float(h)) - (1.0 - texoffset.w)) / edges.w, 0.0, 1.0);

                    // blend function with luminance control (for each of the 3 channels)
                    blend = (a < 0.5) ? (luminance * pow(2.0 * a, exponent))
                                      : one - (one - luminance) * pow(2.0 * (1.0 - a), exponent);

                    // gamma correction (for each of the 3 channels)
                    blend = pow(blend, one / gamma);
                }

                // set final color
                gl_FragColor = vec4(col.rgb * blend, col.a * 1.0);
            });

        gl3VertexShader = "#version 330\n";
        gl3VertexShader += STRINGIFY(
            uniform mat4 modelViewProjectionMatrix;

            layout(location = 0) in vec3 position;
            layout(location = 3) in vec3 texcoord;

            out vec3 texCoordVarying;

            void main() {
                gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
                texCoordVarying = texcoord;
            });

        gl3FragmentShader = "#version 330\n";
        gl3FragmentShader += STRINGIFY(

            uniform sampler2D tex0;

            // used in a few inversions
            const vec3 one = vec3(1.0);

            // controls the interpolation curve ([1..n], 1.0 = linear, 2.0 = default quadratic)
            uniform float exponent; // try: 2.0;
            // controls the center of interpolation ([0..1], 0.5 = linear)
            uniform vec3 luminance; // try: vec3(0.5);
            // controls gamma levels ([1..n], 1.8 or 2.2 is typical)
            uniform vec3 gamma; // try: vec3(1.8, 1.5, 1.2);
            // controls blending area at left, top, right and bottom in percentages ([0..0.5])
            uniform vec4 edges; // try: vec4(0.4, 0.4, 0.0, 0.0);
            uniform vec4 texoffset;
            uniform int w;
            uniform int h;
            uniform bool edgeBlend;
            in vec3 texCoordVarying;
            out vec4 fragColor;

            void main(void) {
                // initialize coordinates and colors
                vec2 uv = texCoordVarying.xy / texCoordVarying.z;
                vec4 col = texture(tex0, uv);

                vec3 blend = vec3(1.0, 1.0, 1.0);

                // calculate edge blending factor
                if (edgeBlend) {
                    float a = 1.0;
                    if (edges.x > 0.0)
                        a *= clamp((uv.x / float(w) - texoffset.x) / edges.x, 0.0, 1.0);
                    if (edges.y > 0.0)
                        a *= clamp((uv.y / float(h) - texoffset.y) / edges.y, 0.0, 1.0);
                    if (edges.z > 0.0)
                        a *= clamp((1.0 - (uv.x / float(w)) - (1.0 - texoffset.z)) / edges.z, 0.0, 1.0);
                    if (edges.w > 0.0)
                        a *= clamp((1.0 - (uv.y / float(h)) - (1.0 - texoffset.w)) / edges.w, 0.0, 1.0);

                    // blend function with luminance control (for each of the 3 channels)
                    blend = (a < 0.5) ? (luminance * pow(2.0 * a, exponent))
                                           : one - (one - luminance) * pow(2.0 * (1.0 - a), exponent);

                    // gamma correction (for each of the 3 channels)
                    blend = pow(blend, one / gamma);
                }

                // set final color
                fragColor = vec4(col.rgb * blend, col.a * 1.0);
            });

        gl2FragmentShader = "#version 120\n #extension GL_ARB_texture_rectangle : enable\n";
        gl2FragmentShader += STRINGIFY(
            uniform sampler2DRect tex0;

            // used in a few inversions
            const vec3 one = vec3(1.0);

            // controls the interpolation curve ([1..n], 1.0 = linear, 2.0 = default quadratic)
            uniform float exponent; // try: 2.0;
            // controls the center of interpolation ([0..1], 0.5 = linear)
            uniform vec3 luminance; // try: vec3(0.5);
            // controls gamma levels ([1..n], 1.8 or 2.2 is typical)
            uniform vec3 gamma; // try: vec3(1.8, 1.5, 1.2);
            // controls blending area at left, top, right and bottom in percentages ([0..0.5])
            uniform vec4 edges; // try: vec4(0.4, 0.4, 0.0, 0.0);
            uniform vec4 texoffset;
            uniform int w;
            uniform int h;

            void main(void) {
                vec2 uv = gl_TexCoord[0].xy;
                vec4 col = texture2DRect(tex0, uv);

                // calculate edge blending factor
                float a = 1.0;
                if (edges.x > 0.0)
                    a *= clamp((uv.x / float(w) - texoffset.x) / edges.x, 0.0, 1.0);
                if (edges.y > 0.0)
                    a *= clamp((uv.y / float(h) - texoffset.y) / edges.y, 0.0, 1.0);
                if (edges.z > 0.0)
                    a *= clamp((1.0 - (uv.x / float(w)) - (1.0 - texoffset.z)) / edges.z, 0.0, 1.0);
                if (edges.w > 0.0)
                    a *= clamp((1.0 - (uv.y / float(h)) - (1.0 - texoffset.w)) / edges.w, 0.0, 1.0);

                // blend function with luminance control (for each of the 3 channels)
                vec3 blend = (a < 0.5) ? (luminance * pow(2.0 * a, exponent))
                                       : one - (one - luminance) * pow(2.0 * (1.0 - a), exponent);

                // gamma correction (for each of the 3 channels)
                blend = pow(blend, one / gamma);

                gl_FragColor = vec4(col.rgb * blend, col.a * 1.0);
            });

#ifdef TARGET_OPENGLES
		if(ofIsGLProgrammableRenderer()) {
			ofLogNotice() << "Setting up GLES2 shader for QuadSurface";
			shader.setupShaderFromSource(GL_VERTEX_SHADER, glESVertexShader);
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, glESFragmentShader);
			shader.linkProgram();
			shader.begin();
			shader.setUniform1f("exponent", 1.0f);
			shader.setUniform3f("luminance", 0.5, 0.5, 0.5);
			shader.setUniform3f("gamma", 1.8, 1.8, 1.8);
			shader.end();

			v3PosAttributeIndex = glGetAttribLocation(shader.getProgram(), "position");
			v3TexAttributeIndex = glGetAttribLocation(shader.getProgram(), "texcoord");
		}
#else
        if (ofIsGLProgrammableRenderer()) {
            shader.setupShaderFromSource(GL_VERTEX_SHADER, gl3VertexShader);
            shader.setupShaderFromSource(GL_FRAGMENT_SHADER, gl3FragmentShader);
            shader.linkProgram();
            shader.begin();
            shader.setUniform1f("exponent", 1.0f);
            shader.setUniform3f("luminance", 0.5, 0.5, 0.5);
            shader.setUniform3f("gamma", 1.8, 1.8, 1.8);
            shader.end();
        } else {
            shader.setupShaderFromSource(GL_FRAGMENT_SHADER, gl2FragmentShader);
            shader.linkProgram();
            shader.begin();
            shader.setUniform1f("exponent", 1.0f);
            shader.setUniform3f("luminance", 0.5, 0.5, 0.5);
            shader.setUniform3f("gamma", 1.8, 1.8, 1.8);
            shader.end();
        }

#endif
    }

    void QuadSurface::calculateQ()
    {
        float ax = getVertex(2).x - getVertex(0).x;
        float ay = getVertex(2).y - getVertex(0).y;
        float bx = getVertex(3).x - getVertex(1).x;
        float by = getVertex(3).y - getVertex(1).y;

        float cross = ax * by - ay * bx;

        if (cross != 0) {
            float cy = getVertex(0).y - getVertex(1).y;
            float cx = getVertex(0).x - getVertex(1).x;

            float s = (ax * cy - ay * cx) / cross;

            if (s > 0 && s < 1) {
                float t = (bx * cy - by * cx) / cross;

                if (t > 0 && t < 1) {
                    q0 = 1 / (1 - t);
                    q1 = 1 / (1 - s);
                    q2 = 1 / t;
                    q3 = 1 / s;
                    // you can now pass (u * q, v * q, q) to OpenGL
                }
            }
        }
    }

    //--------------------------------------------------------------
    void QuadSurface::setupVertexArray()
    {
        q0 = q1 = q2 = q3 = 1.0;

        GLfloat v[] = { getVertex(0).x, getVertex(0).y, getVertex(1).x, getVertex(1).y,
            getVertex(2).x, getVertex(2).y, getVertex(3).x, getVertex(3).y,
            getTexCoord(0).x * q0, getTexCoord(0).y * q0, q0,
            getTexCoord(1).x * q1, getTexCoord(1).y * q1, q1,
            getTexCoord(2).x * q2, getTexCoord(2).y * q2, q2,
            getTexCoord(3).x * q3, getTexCoord(3).y * q3, q3 };

#ifndef TARGET_OPENGLES
        if (ofIsGLProgrammableRenderer()) {
            glGenVertexArrays(1, &const_cast<QuadSurface*>(this)->VAO);
        }
#endif

        glGenBuffers(1, &VBO);

#ifndef TARGET_OPENGLES
        if (ofIsGLProgrammableRenderer()) {
            glBindVertexArray(VAO);
        }
#endif

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

#ifndef TARGET_OPENGLES
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
#endif
        glBindBuffer(GL_ARRAY_BUFFER, 0);

#ifndef TARGET_OPENGLES
        if (ofIsGLProgrammableRenderer()) {
            glBindVertexArray(0);
        }
#endif
    }

    //--------------------------------------------------------------
    void QuadSurface::updateVertexBuffer()
    {
        GLfloat v[] = { getVertex(0).x, getVertex(0).y, getVertex(1).x, getVertex(1).y,
            getVertex(2).x, getVertex(2).y, getVertex(3).x, getVertex(3).y,
            getTexCoord(0).x * q0, getTexCoord(0).y * q0, q0,
            getTexCoord(1).x * q1, getTexCoord(1).y * q1, q1,
            getTexCoord(2).x * q2, getTexCoord(2).y * q2, q2,
            getTexCoord(3).x * q3, getTexCoord(3).y * q3, q3 };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace piMapper
} // namespace ofx
