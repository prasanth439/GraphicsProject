#pragma once
#include "engine.h"
#include "runscript.h"
#include <list>
#include <vector>
#include <map>
class SceneObj;
struct PosComp{
    bool operator()(const glm::ivec2& a,const glm::ivec2& b){
        return (a.x<b.x)||((a.x==b.x)&&(a.y<b.y));
    }
};
class Terrain:public RunScript{
public:
    Terrain();
    Terrain(int);
    ~Terrain();
    void init();
    void update(float time_step);
    SceneObj* CreateTerrain(const glm::ivec2& pos);
private:
    std::list<SceneObj*>* availableTerrain;
    std::map<glm::ivec2,SceneObj*,PosComp>* terrainMapPrev,*terrainMapCurr;
    float sizeOfFloor;
public:
    SceneObj* gridParent;
    SceneObj* terrain_obj;
};