#include "QuadSurface.h"

namespace ofx {
namespace piMapper {

QuadSurface::QuadSurface(){
	_perspectiveWarping = false;
    _edgeBlendingMode = false;
	setup();
}

QuadSurface::~QuadSurface(){
	std::cout << "QuadSurface destructor." << std::endl;
}

void QuadSurface::setup(){
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

    setupShaders();
}

void QuadSurface::setup(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4,
						Vec2 t1, Vec2 t2, Vec2 t3, Vec2 t4,
						BaseSource * newSource){
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
	calculateHomography();
}

void QuadSurface::draw(){
	if(source->getTexture() == 0){
		return;
	}
	
	if(!source->getTexture()->isAllocated()){
		return;
	}

	if(_perspectiveWarping){
		bool meshChanged = false;
		
		if(
			mesh.getVertices()[0] != _meshCache.getVertices()[0] ||
			mesh.getVertices()[1] != _meshCache.getVertices()[1] ||
			mesh.getVertices()[2] != _meshCache.getVertices()[2] ||
			mesh.getVertices()[3] != _meshCache.getVertices()[3])
		{
			meshChanged = true;
		}
		
		if(meshChanged){
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
		if(true){
			bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
			ofEnableNormalizedTexCoords();
			
			glMultMatrixf(_matrix);
            if(_edgeBlendingMode) {
                edgeBlendShader.begin();
                edgeBlendShader.setUniformTexture("tex0", *(source->getTexture()) , 1 );
                edgeBlendShader.setUniform1i("w", source->getTexture()->getWidth());
                edgeBlendShader.setUniform1i("h", source->getTexture()->getHeight());
            }
			source->getTexture()->bind();
			m.draw();
			source->getTexture()->unbind();
            if(_edgeBlendingMode) {
                edgeBlendShader.end();
            }
			
			if(!normalizedTexCoords){
				ofDisableNormalizedTexCoords();
			}
		}
		ofPopMatrix();
	}else{
		bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
		ofEnableNormalizedTexCoords();
		
		ofPushStyle();
		ofSetColor(255, 255, 255);
	
		source->getTexture()->bind();
		mesh.draw();
		source->getTexture()->unbind();
		
		ofPopStyle();
		
		if(!normalizedTexCoords){
			ofDisableNormalizedTexCoords();
		}
	}
}

void QuadSurface::setVertex(int index, Vec3 p){
	if(index > 3){
		ofLog() << "Vertex with this index does not exist: " << index << std::endl;
		return;
	}

	mesh.setVertex(index, p.toOf());
	ofNotifyEvent(vertexChangedEvent, index, this);
}

void QuadSurface::setVertices(std::vector<Vec3> v){
	if(v.size() != 4){
        throw runtime_error("Wrong numexponentber of vertices");
	}
	
	for(int i = 0; i < 4; ++i){
		mesh.setVertex(i, v[i].toOf());
	}
	
	std::vector<Vec3> vertices = Vec3::fromOf(mesh.getVertices());
	ofNotifyEvent(verticesChangedEvent, vertices, this);
}

void QuadSurface::setTexCoord(int index, Vec2 t){
	if(index > 3){
		ofLog() << "Texture coordinate with this index does not exist: " << index
				<< std::endl;
		return;
	}

	mesh.setTexCoord(index, t.toOf());
}

void QuadSurface::setTexCoords(std::vector<Vec2> t){
	if(t.size() != 4){
		throw runtime_error("Wrong number of vertices");
	}
	for(int i = 0; i < 4; ++i){
		mesh.setTexCoord(i, t[i].toOf());
	}
}

void QuadSurface::moveBy(Vec3 v){
	for(int i = 0; i < mesh.getVertices().size(); i++){
		mesh.getVertices()[i] += v.toOf();
	}
	
	setMoved(true);
	std::vector<Vec3> vertices = Vec3::fromOf(mesh.getVertices());
	ofNotifyEvent(verticesChangedEvent, vertices, this);
}

int QuadSurface::getType(){
	return SurfaceType::QUAD_SURFACE;
}

bool QuadSurface::hitTest(Vec2 p){
	ofPolyline line = getHitArea();

	if(line.inside(p.x, p.y)){
		return true;
	}else{
		return false;
	}
}

Vec3 QuadSurface::getVertex(int index){
	if(index > 3){
		ofLog() << "Vertex with this index does not exist: " << index << std::endl;
		throw runtime_error("Vertex index out of bounds.");
	}
	
	return Vec3(
		mesh.getVertex(index).x,
		mesh.getVertex(index).y,
		mesh.getVertex(index).z);
}

Vec2 QuadSurface::getTexCoord(int index){
	if(index > 3){
		throw runtime_error("Texture coordinate index out of bounds.");
	}

	return Vec2(
		mesh.getTexCoord(index).x,
		mesh.getTexCoord(index).y);
}

ofPolyline QuadSurface::getHitArea(){
	ofPolyline line;
	
	line.addVertex(ofPoint(mesh.getVertex(0).x, mesh.getVertex(0).y));
	line.addVertex(ofPoint(mesh.getVertex(1).x, mesh.getVertex(1).y));
	line.addVertex(ofPoint(mesh.getVertex(2).x, mesh.getVertex(2).y));
	line.addVertex(ofPoint(mesh.getVertex(3).x, mesh.getVertex(3).y));
	line.close();

	return line;
}

ofPolyline QuadSurface::getTextureHitArea(){
	ofPolyline line;
	Vec2 textureSize =
		Vec2(source->getTexture()->getWidth(),
		source->getTexture()->getHeight());
	for(int i = 0; i < mesh.getTexCoords().size(); i++){
		line.addVertex(ofPoint(mesh.getTexCoords()[i] * textureSize.toOf()));
	}
	line.close();

	return line;
}

std::vector<Vec3> QuadSurface::getVertices(){
	return Vec3::fromOf(mesh.getVertices());
}

std::vector<Vec2> QuadSurface::getTexCoords(){
	return Vec2::fromOf(mesh.getTexCoords());
}

void QuadSurface::calculateHomography(){
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

void QuadSurface::setPerspectiveWarping(bool b){
	_perspectiveWarping = b;
}

bool QuadSurface::getPerspectiveWarping(){
	return _perspectiveWarping;
}

void QuadSurface::setEdgeBlendMode(bool b){
    _edgeBlendingMode = b;
}

ofRectangle QuadSurface::getMeshBoundingBox(){
	float minX = 10000.0f;
	float minY = 10000.0f;
	float maxX = 0.0f;
	float maxY = 0.0f;
	
	for(int i = 0; i < mesh.getVertices().size(); ++i){
		if(mesh.getVertices()[i].x < minX){
			minX = mesh.getVertices()[i].x;
		}
		
		if(mesh.getVertices()[i].y < minY){
			minY = mesh.getVertices()[i].y;
		}
		
		if(mesh.getVertices()[i].x > maxX){
			maxX = mesh.getVertices()[i].x;
		}
		
		if(mesh.getVertices()[i].y > maxY){
			maxY = mesh.getVertices()[i].y;
		}
	}
	
	ofRectangle boundingBox = ofRectangle(ofPoint(minX, minY), ofPoint(maxX, maxY));
	return boundingBox;
}

BaseSurface * QuadSurface::clone(){
	QuadSurface * s = new QuadSurface();
	s->setVertices(getVertices());
	s->setTexCoords(getTexCoords());
	s->setPerspectiveWarping(getPerspectiveWarping());
    BaseSource * src = getSource();
    src->referenceCount++;
	s->setSource(src);
	return s;
}

#define STRINGIFY(A) #A
void QuadSurface::setupShaders()
{
    glESVertexShader = STRINGIFY(
                attribute vec4 position;
                attribute vec4 color;
                attribute vec4 normal;
                attribute vec2 texcoord;

                uniform mat4 modelViewMatrix;
                uniform mat4 projectionMatrix;
                uniform sampler2D maskTex;

                varying vec4 colorVarying;
                varying vec2 texCoordVarying;

                void main() {

                    //get our current vertex position so we can modify it
                    vec4 pos = projectionMatrix*modelViewMatrix*position;

                    gl_Position = pos;
                    colorVarying = color;
                    texCoordVarying = texcoord;
                }
        );

        glESFragmentShader = STRINGIFY(
    #ifdef GL_ES
                // define default precision for float, vec, mat.
                precision highp float;
    #endif

                uniform sampler2D tex0;
                uniform vec4 globalColor; // ????

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

                varying vec2 texCoordVarying;

                void main (void)
                {
                    // initialize coordinates and colors
                    vec2 uv = texCoordVarying
                    vec3 col = texture2D(tex0,uv).rgb;

                    // calculate edge blending factor
                    float a = 1.0;
                    if(edges.x > 0.0) a *= clamp(uv.x/edges.x, 0.0, 1.0);
                    if(edges.y > 0.0) a *= clamp(uv.y/edges.y, 0.0, 1.0);
                    if(edges.z > 0.0) a *= clamp((1.0-uv.x)/edges.z, 0.0, 1.0);
                    if(edges.w > 0.0) a *= clamp((1.0-uv.y)/edges.w, 0.0, 1.0);


                    // blend function with luminance control (for each of the 3 channels)
                    vec3 blend = (a < 0.5) ? (luminance * pow(2.0 * a, exponent))
                        : one - (one - luminance) * pow(2.0 * (1.0 - a), exponent);

                    // gamma correction (for each of the 3 channels)
                    blend = pow(blend, one / gamma);

                    // set final color
                    gl_FragColor = vec4(col * blend, 1.0);
                }
        );

        gl2FragmentShader = "#version 120\n #extension GL_ARB_texture_rectangle : enable\n";
        gl2FragmentShader += STRINGIFY(
            uniform sampler2DRect tex0;
            //uniform sampler2D tex0;

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
            uniform int w;
            uniform int h;

            void main(void)
            {
                // initialize coordinates and colors
                vec2 uv = gl_TexCoord[0].xy;
                vec4 col = texture2DRect(tex0,uv);

                // calculate edge blending factor
                float a = 1.0;
//                if(edges.x > 0.0) a *= clamp(uv.x/edges.x, 0.0, 1.0);
//                if(edges.y > 0.0) a *= clamp(uv.y/edges.y, 0.0, 1.0);
//                if(edges.z > 0.0) a *= clamp((1.0-uv.x)/edges.z, 0.0, 1.0);
//                if(edges.w > 0.0) a *= clamp((1.0-uv.y)/edges.w, 0.0, 1.0);
                if(edges.x > 0.0) a *= clamp((uv.x/float(w))/edges.x, 0.0, 1.0);
                if(edges.y > 0.0) a *= clamp((uv.y/float(h))/edges.y, 0.0, 1.0);
                if(edges.z > 0.0) a *= clamp((1.0-(uv.x/float(w)))/edges.z, 0.0, 1.0);
                if(edges.w > 0.0) a *= clamp((1.0-(uv.y/float(h)))/edges.w, 0.0, 1.0);

                // blend function with luminance control (for each of the 3 channels)
                vec3 blend = (a < 0.5) ? (luminance * pow(2.0 * a, exponent))
                    : one - (one - luminance) * pow(2.0 * (1.0 - a), exponent);

                // gamma correction (for each of the 3 channels)
                blend = pow(blend, one / gamma);

                // set final color
                gl_FragColor = vec4(col.rgb * blend, col.a*1.0);
                //gl_FragColor = vec4(col, 1.0);
            }
        );

    #ifdef TARGET_OPENGLES
        edgeBlendShader.setupShaderFromSource(GL_VERTEX_SHADER, glESVertexShader);
        edgeBlendShader.setupShaderFromSource(GL_FRAGMENT_SHADER, glESFragmentShader);
        edgeBlendShader.bindDefaults();
        edgeBlendShader.linkProgram();
    #else
        if (ofIsGLProgrammableRenderer()) {

        } else {
            edgeBlendShader.setupShaderFromSource(GL_FRAGMENT_SHADER, gl2FragmentShader);
            edgeBlendShader.linkProgram();
            edgeBlendShader.begin();
            edgeBlendShader.setUniform1f("exponent",1.0f);
            edgeBlendShader.setUniform3f("luminance",0.5,0.5,0.5);
            //edgeBlendShader.setUniform3f("gamma",1.0,1.0,1.0);
            edgeBlendShader.setUniform3f("gamma",1.8,1.8,1.8);
            edgeBlendShader.setUniform4f("edges",0.2,0.0,0.0,0.0);
            edgeBlendShader.end();
        }
    #endif
}

} // namespace piMapper
} // namespace ofx
