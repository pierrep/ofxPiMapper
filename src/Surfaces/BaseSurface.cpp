#include "BaseSurface.h"

namespace ofx {
namespace piMapper {

BaseSurface::BaseSurface(){
	_moved = false;
	_scale = 1.0f;
	createDefaultTexture();
}

BaseSurface::~BaseSurface(){
	delete defaultSource;
	defaultSource = 0;
	defaultTexture.clear();
}

void BaseSurface::createDefaultTexture(){
	ofPixels pixels;
	pixels.allocate(500, 500, 1);
	for(int i = 0; i < pixels.size(); i++){
		pixels[i] = 255;
	}
	
	// size of each test pattern square
	int squareSize = 10;

	bool sy = false;
	for(int y = 0; y < pixels.getWidth(); y += squareSize){
		bool sx = false;
		for(int x = 0; x < pixels.getHeight(); x += squareSize){
			for(int yi = 0; yi < squareSize; yi++){
				for(int xi = 0; xi < squareSize; xi++){
					if(sx && sy){
						pixels[(y + yi) * pixels.getWidth() + x + xi] = 255;
					}else if(sx && !sy){
						pixels[(y + yi) * pixels.getWidth() + x + xi] = 0;
					}else if(!sx && sy){
						pixels[(y + yi) * pixels.getWidth() + x + xi] = 0;
					}else{
						pixels[(y + yi) * pixels.getWidth() + x + xi] = 255;
					}
				}
			}
			sx = !sx;
		}
		sy = !sy;
	}

	// load pixels into texture
	defaultTexture.loadData(pixels);
	
	// Create new source to be the default
	defaultSource = new BaseSource(&defaultTexture);
	source = defaultSource;
}

void BaseSurface::drawTexture(Vec3 position){
	if(source->getTexture() == 0){
		ofLogWarning("BaseSurface") << "Source texture empty. Not drawing.";
		return;
	}

    bool normalizedTexCoords = ofGetUsingNormalizedTexCoords();
    ofDisableNormalizedTexCoords();
    source->getTexture()->draw(position.toOf(),source->getTexture()->getWidth(),source->getTexture()->getHeight());
    if(normalizedTexCoords) {
        ofEnableNormalizedTexCoords();
    }
}

void BaseSurface::setSource(BaseSource * newSource){
	source = newSource;
}

BaseSource * BaseSurface::getSource(){
	return source;
}

BaseSource * BaseSurface::getDefaultSource(){
	return defaultSource;
}

void BaseSurface::setMoved(bool moved){
	_moved = moved;
}

void BaseSurface::scaleTo(float scale){
	Vec3 centroid(
		getBoundingBox().getCenter().x,
		getBoundingBox().getCenter().y,
		getBoundingBox().getCenter().z);
	for(unsigned int i = 0; i < mesh.getVertices().size(); ++i){
		Vec3 d = (Vec3(mesh.getVertices()[i]) - centroid) / _scale;
		d *= scale;
		mesh.getVertices()[i] = (centroid + d).toOf();
	}
	
	_scale = scale;
	
	std::vector<Vec3> vertices = Vec3::fromOf(mesh.getVertices());
	ofNotifyEvent(verticesChangedEvent, vertices, this);
}

bool BaseSurface::getMoved(){
	return _moved;
}

float BaseSurface::getScale(){
	return _scale;
}

ofMesh & BaseSurface::getMesh(){
	return mesh;
}

ofRectangle & BaseSurface::getBoundingBox(){
	
	// Get top left
	_boundingBox.x = 999999;
	_boundingBox.y = 999999;
	for(unsigned int i = 0; i < mesh.getVertices().size(); ++i){
		if(mesh.getVertices()[i].x < _boundingBox.x){
			_boundingBox.x = mesh.getVertices()[i].x;
		}
		
		if(mesh.getVertices()[i].y < _boundingBox.y){
			_boundingBox.y = mesh.getVertices()[i].y;
		}
	}
	
	// Get bottom right
	_boundingBox.width = -999999;
	_boundingBox.height = -999999;
	for(unsigned int i = 0; i < mesh.getVertices().size(); ++i){
		if(mesh.getVertices()[i].x > _boundingBox.x + _boundingBox.width){
			_boundingBox.width = mesh.getVertices()[i].x - _boundingBox.x;
		}
		
		if(mesh.getVertices()[i].y > _boundingBox.y + _boundingBox.height){
			_boundingBox.height = mesh.getVertices()[i].y - _boundingBox.y;
		}
	}

	return _boundingBox;
}

} // namespace piMapper
} // namespace ofx
