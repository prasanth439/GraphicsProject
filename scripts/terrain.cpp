#include "terrain.h"

Terrain::Terrain(){
    sizeOfGrid = 40;
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
    newTerrain->transform->setPosition(glm::vec3(pos.x*sizeOfGrid,0,pos.y*sizeOfGrid));
    availableTerrain->push_back(newTerrain);
    newTerrain->shouldDisplay = true;
    return newTerrain;
}

void Terrain::update(float time_step){
    glm::ivec2 pos(glm::floor(scene_obj->transform->position.x/sizeOfGrid),glm::floor(scene_obj->transform->position.z/sizeOfGrid));
    
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
    for(auto a=availableTerrain->begin();a!=availableTerrain->end();){
        if(!(*a)->shouldDisplay){
            delete (*a);
            a=availableTerrain->erase(a);
        }
        else{
            ++a;
        }
    }
    
    auto tempReference = terrainMapPrev;
    terrainMapPrev = terrainMapCurr;
    terrainMapCurr = tempReference;
    terrainMapCurr->clear();
}
