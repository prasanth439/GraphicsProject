#include "sceneobj.h"

SceneObj::SceneObj():SceneObj(nullptr,nullptr){

}
SceneObj::~SceneObj(){
    delete transform;
    delete renderer;
}
SceneObj::SceneObj(Renderer* renderer_,Transform* transform_){
    renderer = renderer_;
    transform = transform_;
    shouldDisplay = true;
}
SceneObj* SceneObj::init()
{
    renderer->init(this);
    transform->init(this);
    return this;
}
SceneObj* SceneObj::clone(SceneObj* obj){
    SceneObj* newObj = (new SceneObj(obj->renderer->clone(),Transform::clone(obj->transform)))->init();
    return newObj;
}
void SceneObj::draw_obj(const glm::mat4& world_mat){
    renderer->render(world_mat);
    return ;
}
