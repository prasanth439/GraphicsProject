#include "sceneobj.h"

SceneObj::SceneObj():SceneObj(nullptr,nullptr){
    renderer = nullptr;
    shader = nullptr;
}
SceneObj::SceneObj(Renderer* renderer_,Shader* shader_){
    renderer = renderer_;
    shader = shader_;
}
SceneObj* SceneObj::init()
{
    renderer->init(this);
    shader->init(this);
    return this;
}

void SceneObj::draw_obj(){
    renderer->render();
    return ;
}