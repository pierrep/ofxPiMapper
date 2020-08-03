#include "ProjectionEditorWidget.h"

namespace ofx {
namespace piMapper {

ProjectionEditorWidget::ProjectionEditorWidget(){
	surfaceManager = 0;
	bShiftKeyDown = false;
	fSnapDistance = 10.0f;
    bEdgeBlendMode = false;
}

void ProjectionEditorWidget::update(){
	// update surface if one of the joints is being dragged
	for(int i = 0; i < joints.size(); i++){
		if(joints[i]->isDragged() || joints[i]->isSelected()){
			if(surfaceManager->getSelectedSurface() != 0){
				// update vertex to new location
				surfaceManager->getSelectedSurface()->setVertex(i, Vec3(
					joints[i]->position.x,
					joints[i]->position.y,
					0.0f));
			}else{
				// clear joints if there is no surface selected
				// as the remove selected surface in the surface manager
				// is not supposed to access joints here
				joints.clear();
			}
			break;
		}
	}

    for(int i = 0; i < edgeBlendJoints.size(); i++){
        if(edgeBlendJoints[i]->isDragged() || edgeBlendJoints[i]->isSelected()){
            if(surfaceManager->getSelectedSurface() != 0){
                // update Edge Blend joints
                if(edgeBlendJoints[i]->isDragged()) {
                    setBlendEdges();
                }
            }else{
                // clear joints if there is no surface selected
                // as the remove selected surface in the surface manager
                // is not supposed to access joints here
                edgeBlendJoints.clear();
            }
            break;
        }
    }
}

void ProjectionEditorWidget::draw(){
	if(surfaceManager == 0){
		return;
	}
	if(surfaceManager->getSelectedSurface() == 0){
		return;
	}
	if(joints.size() <= 0){
		createJoints();
	}
	drawJoints();

    // Soft edge blending
    if(bEdgeBlendMode) {
        if(edgeBlendJoints.size() <= 0){
            createEdgeBlendJoints();
        }
        drawEdgeBlendJoints();
    }
}

void ProjectionEditorWidget::mouseDragged(ofMouseEventArgs & args){
	
    // Pass args to joints
	for(unsigned int i = 0; i < joints.size(); ++i){
		joints[i]->mouseDragged(args);
	}

    // Pass args to edge blend joints
    if(bEdgeBlendMode) {
        for(unsigned int i = 0; i < edgeBlendJoints.size(); ++i){
            edgeBlendJoints[i]->mouseDragged(args);
        }
    }

	Vec2 mousePosition = Vec2(args.x, args.y);

	// Collect all vertices of the projection surfaces
	std::vector<Vec3> allVertices;
	for(int i = 0; i < surfaceManager->size(); i++){
		BaseSurface * surface = surfaceManager->getSurface(i);
		
		if(surface == surfaceManager->getSelectedSurface()){
			continue; // Don't add vertices of the selected surface
		}
		
		for(int j = 0; j < surface->getVertices().size(); j++){
			allVertices.push_back(surface->getVertices()[j]);
		}
	}

	// Snap currently dragged joint to nearest vertex
	for(int i = 0; i < joints.size(); i++){
		if(joints[i]->isDragged()){
			for(int j = 0; j < allVertices.size(); j++){
				Vec2 v(allVertices[j].x, allVertices[j].y);
				float distance = mousePosition.distance(v);
				
				if(distance < fSnapDistance){
					joints[i]->position = v;
					Vec2 clickDistance = joints[i]->position - Vec2(args.x, args.y);
					joints[i]->setClickDistance(clickDistance);
					break;
				}
			}
		}
	}
}

void ProjectionEditorWidget::gotMessage(ofMessage & msg){
	if(msg.message == "surfaceSelected"){
		clearJoints();
		createJoints();
	}
}

void ProjectionEditorWidget::setSurfaceManager(SurfaceManager * newSurfaceManager){
	if(surfaceManager != 0){
		ofRemoveListener(surfaceManager->vertexChangedEvent, this,
			&ProjectionEditorWidget::onVertexChanged);
		ofRemoveListener(surfaceManager->verticesChangedEvent, this,
			&ProjectionEditorWidget::onVerticesChanged);
		ofRemoveListener(surfaceManager->surfaceSelectedEvent, this,
			&ProjectionEditorWidget::onSurfaceSelected);
		ofRemoveListener(surfaceManager->vertexSelectedEvent, this,
			&ProjectionEditorWidget::onVertexSelected);
		ofRemoveListener(surfaceManager->vertexUnselectedEvent, this,
			&ProjectionEditorWidget::onVertexUnselected);
	}
	
	surfaceManager = newSurfaceManager;
	
	ofAddListener(surfaceManager->vertexChangedEvent, this,
		&ProjectionEditorWidget::onVertexChanged);
	ofAddListener(surfaceManager->verticesChangedEvent, this,
		&ProjectionEditorWidget::onVerticesChanged);
	ofAddListener(surfaceManager->surfaceSelectedEvent, this,
		&ProjectionEditorWidget::onSurfaceSelected);
	ofAddListener(surfaceManager->vertexSelectedEvent, this,
		&ProjectionEditorWidget::onVertexSelected);
	ofAddListener(surfaceManager->vertexUnselectedEvent, this,
		&ProjectionEditorWidget::onVertexUnselected);
}

void ProjectionEditorWidget::setBlendEdges(){
    bEnableEdgeBlend = true;

    QuadSurface * quad = dynamic_cast<QuadSurface *>(surfaceManager->getSelectedSurface());
    float w = surfaceManager->getSelectedSurface()->getSource()->getTexture()->getWidth();
    float h = surfaceManager->getSelectedSurface()->getSource()->getTexture()->getHeight();
    float left = (edgeBlendJoints[3]->getDragPosition().x - edgeBlendJoints[3]->position.x)/w;
    float top = (edgeBlendJoints[0]->getDragPosition().y - edgeBlendJoints[0]->position.y)/h;
    float right = (edgeBlendJoints[1]->position.x - edgeBlendJoints[1]->getDragPosition().x)/w;
    float bottom = (edgeBlendJoints[2]->position.y - edgeBlendJoints[2]->getDragPosition().y)/h;
    quad->setEdges(ofVec4f(left,top,right,bottom));

    //ofLogNotice() << "setEdges  left: " << left << " top: " << top << " right: " << right << " bottom: " << bottom;

    if((left <= 0.0f) && (top <= 0.0f) && (right <= 0.0f) && (bottom <= 0.0f)) {
        bEnableEdgeBlend = false;
        ofLogNotice()<< "Edge blend disabled";
    }

    quad->setEdgeBlending(bEnableEdgeBlend);

}

void ProjectionEditorWidget::clearJoints(){
	while(joints.size()){
		delete joints.back();
		joints.pop_back();
	}
}

void ProjectionEditorWidget::createJoints(){
	if(surfaceManager == 0){
		return;
	}
	clearJoints();

	if(surfaceManager->getSelectedSurface() == 0){
		ofLog(OF_LOG_WARNING, "Trying to create joints while no surface selected.");
		return;
	}

	std::vector<Vec3> vertices = surfaceManager->getSelectedSurface()->getVertices();

	for(int i = 0; i < vertices.size(); i++){
		joints.push_back(new CircleJoint());
		joints.back()->position = Vec2(vertices[i].x, vertices[i].y);
	}
}

void ProjectionEditorWidget::updateJoints(){
	if(surfaceManager->getSelectedSurface()){
		std::vector<Vec3> vertices = surfaceManager->getSelectedSurface()->getVertices();
		
		for(int i = 0; i < vertices.size(); i++){
			joints[i]->position = Vec2(vertices[i].x, vertices[i].y);
		}
	}

}

void ProjectionEditorWidget::unselectAllJoints(){
	for(int i = 0; i < joints.size(); i++){
		joints[i]->unselect();
	}

    if(bEdgeBlendMode) {        //ofEvent <int> edgeBlendJointSelectedEvent;
        //ofEvent <int> edgeBlendJointUnselectedEvent;
        for(int i = 0; i < edgeBlendJoints.size(); i++){
            edgeBlendJoints[i]->unselect();
        };
    }
}

void ProjectionEditorWidget::moveSelectedSurface(Vec3 by){
	if(surfaceManager == 0){
		return;
	}
	
	if(surfaceManager->getSelectedSurface() == 0){
		return;
	}
	
	surfaceManager->getSelectedSurface()->moveBy(by);
	updateJoints();

    if(bEdgeBlendMode) {
       updateEdgeBlendJoints();
    }
}

void ProjectionEditorWidget::stopDragJoints(){
	for(int i = 0; i < joints.size(); i++){
		joints[i]->stopDrag();
	}

    if(bEdgeBlendMode) {
        for(int i = 0; i < edgeBlendJoints.size(); i++){
            edgeBlendJoints[i]->stopDrag();
        };
    }
}

void ProjectionEditorWidget::setSnapDistance(float newSnapDistance){
	fSnapDistance = newSnapDistance;
}

CircleJoint * ProjectionEditorWidget::hitTestJoints(Vec2 pos){
	if(surfaceManager->getSelectedSurface() == 0){
		return 0;
	}
	for(int i = 0; i < joints.size(); i++){
		if(joints[i]->hitTest(pos)){
			return joints[i];
		}
	}
	return 0;
}

EdgeBlendJoint * ProjectionEditorWidget::hitTestEdgeBlendJoints(Vec2 pos){
    if(surfaceManager->getSelectedSurface() == 0){
        return 0;
    }
    if(!bEdgeBlendMode){
        return 0;
    }
    for(int i = 0; i < edgeBlendJoints.size(); i++){
        if(edgeBlendJoints[i]->hitTest(pos)){
            return edgeBlendJoints[i];
        }
    }
    return 0;
}

std::vector<CircleJoint *> * ProjectionEditorWidget::getJoints(){
	return &joints;
}

const bool ProjectionEditorWidget::getEdgeBlendMode(){
    return bEdgeBlendMode;
}

void ProjectionEditorWidget::toggleEdgeBlendMode(){
    if(surfaceManager->getSelectedSurface() == 0){
        return;
    }
    if(surfaceManager->getSelectedSurface()->getType() == SurfaceType::QUAD_SURFACE){
        bEdgeBlendMode = !bEdgeBlendMode;        
    }
}

void ProjectionEditorWidget::clearEdgeBlendJoints(){
    while(edgeBlendJoints.size()){
        delete edgeBlendJoints.back();
        edgeBlendJoints.pop_back();
    }
    bEnableEdgeBlend = false;
}

void ProjectionEditorWidget::createEdgeBlendJoints(){
    if(surfaceManager == 0){
        return;
    }

    if(surfaceManager->getSelectedSurface() == 0){
        ofLog(OF_LOG_WARNING, "Trying to create edge blend joints while no surface selected.");
        clearEdgeBlendJoints();
        return;
    }

    //QuadSurface * qs = (QuadSurface *)surfaceManager->getSelectedSurface();

    std::vector<Vec3> vertices = surfaceManager->getSelectedSurface()->getVertices();

    for(int i = 0; i < vertices.size(); i++){
        edgeBlendJoints.push_back(new EdgeBlendJoint());
        int j = (i+1) % vertices.size();
        float x = (vertices[i].x + vertices[j].x)/2.0f;
        float y = (vertices[i].y + vertices[j].y)/2.0f;
        edgeBlendJoints.at(i)->position = Vec2(x,y);
        edgeBlendJoints.at(i)->setDragPosition(Vec2(x,y));
    }
}

void ProjectionEditorWidget::updateEdgeBlendJoints(){
    if(surfaceManager->getSelectedSurface()){
        std::vector<Vec3> vertices = surfaceManager->getSelectedSurface()->getVertices();

        for(int i = 0; i < vertices.size(); i++){
            int j = (i+1) % vertices.size();
            float x = (vertices[i].x + vertices[j].x)/2.0f;
            float y = (vertices[i].y + vertices[j].y)/2.0f;
            edgeBlendJoints.at(i)->position = Vec2(x,y);
        }
    }

}

std::vector<EdgeBlendJoint *> * ProjectionEditorWidget::getEdgeBlendJoints(){
    return &edgeBlendJoints;
}

int ProjectionEditorWidget::getSelectedEdgeBlendJoint(){
    for(int i = 0; i < edgeBlendJoints.size(); i++){
        if(edgeBlendJoints[i]->selected){
            return i;
        }
    }
    return -1;
}

void ProjectionEditorWidget::selectEdgeBlendJoint(int index)
{
    if(getJoints()->size() == 0){
        return;
    }

    unselectAllJoints();
    getEdgeBlendJoints()->at(index)->select();
}

void ProjectionEditorWidget::unselectEdgeBlendJoint(int index)
{
    if(getJoints()->size() == 0){
        return;
    }

    unselectAllJoints();
}

void ProjectionEditorWidget::onVertexChanged(int & i){
	bool isDragged = getJoints()->at(i)->isDragged();
	createJoints();
	getJoints()->at(i)->select();
	if(isDragged){
		getJoints()->at(i)->startDrag();
	}else{
		getJoints()->at(i)->stopDrag();
	}

    if(bEdgeBlendMode) {
        createEdgeBlendJoints();
    }
}

void ProjectionEditorWidget::onVerticesChanged(std::vector<Vec3> & vertices){
	createJoints();

    if(bEdgeBlendMode) {
        createEdgeBlendJoints();
    }
}

void ProjectionEditorWidget::onSurfaceSelected(int & surfaceIndex){
	createJoints();

    if(bEdgeBlendMode) {
        createEdgeBlendJoints();
    }
}

void ProjectionEditorWidget::onVertexSelected(int & vertexIndex){
	if(getJoints()->size() == 0){
		return;
	}
	
	unselectAllJoints();
	getJoints()->at(vertexIndex)->select();
}

void ProjectionEditorWidget::onVertexUnselected(int & vertexIndex){
	if(getJoints()->size() == 0){
		return;
	}
	
	unselectAllJoints();
}

void ProjectionEditorWidget::drawJoints(){
	for(int i = 0; i < joints.size(); i++){
		joints[i]->draw();
	}
}

void ProjectionEditorWidget::drawEdgeBlendJoints(){
    for(int i = 0; i < edgeBlendJoints.size(); i++){
        edgeBlendJoints[i]->draw();
    }
}

} // namespace piMapper
} // namespace ofx
