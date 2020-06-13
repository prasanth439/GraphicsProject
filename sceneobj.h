#pragma once
#include "renderer.h"
#include "shader.h"
#include "transform.h"
#include <list>
class Shader;
class Renderer;

class SceneObj{
    public:
    Renderer* renderer;
    Transform* transform;
    // no need of script
    std::list<SceneObj*> children;
    SceneObj();
    ~SceneObj();
    SceneObj(Renderer* renderer_,Transform* transform_);
    SceneObj* init();
    static SceneObj* clone(SceneObj* obj); 
    void draw_obj(const glm::mat4& world_mat);
    bool shouldDisplay;
};