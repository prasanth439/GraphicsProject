#include "terrain.h"

Terrain::Terrain(){
    sizeOfFloor = 50;
}
Terrain::Terrain(int size_){
    sizeOfFloor = size_;
}
Terrain::~Terrain(){

}

void Terrain::init(){
    terrain_obj->shouldDisplay = false;
    availableTerrain = &gridParent->children;
    terrainMapPrev = new std::map<glm::ivec2,SceneObj*,PosComp>;
    terrainMapCurr = new std::map<glm::ivec2,SceneObj*,PosComp>;
}
SceneObj* Terrain::CreateTerrain(const glm::ivec2& pos){
    SceneObj* newTerrain = SceneObj::clone(terrain_obj);
    newTerrain->transform->setPosition(glm::vec3(pos.x*sizeOfFloor,0,pos.y*sizeOfFloor));
    availableTerrain->push_back(newTerrain);
    newTerrain->shouldDisplay = true;
    return newTerrain;
}

void Terrain::update(float time_step){
    glm::ivec2 pos(glm::floor(scene_obj->transform->position.x/sizeOfFloor+0.5f),glm::floor(scene_obj->transform->position.z/sizeOfFloor+0.5f));
    
    for(auto a=availableTerrain->begin();a!=availableTerrain->end();a++){
        (*a)->shouldDisplay = false;
    }
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            glm::ivec2 newPos = pos + glm::ivec2(i,j);
            auto terrainIter = terrainMapPrev->find(newPos);
            if(terrainIter==terrainMapPrev->end()){
                terrainMapCurr->emplace(newPos,CreateTerrain(newPos));
            }
            else{
                terrainMapCurr->emplace(terrainIter->first,terrainIter->second);
                terrainIter->second->shouldDisplay = true;
            }
        }
    }
    int count_of_display = 0;
    for(auto a=availableTerrain->begin();a!=availableTerrain->end();){
        if(!(*a)->shouldDisplay){
            delete (*a);
            a=availableTerrain->erase(a);
        }
        else{
            ++a;
            count_of_display++;
        }
    }
    assert(availableTerrain->size()==9);
    assert(count_of_display==9);
    auto tempReference = terrainMapPrev;
    terrainMapPrev = terrainMapCurr;
    terrainMapCurr = tempReference;
    terrainMapCurr->clear();
}
