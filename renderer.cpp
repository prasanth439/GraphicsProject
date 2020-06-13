#include "renderer.h"

Renderer::~Renderer(){

}
Renderer::Renderer(Shader* _shader):shader(_shader){

}
Renderer* Renderer::clone(){
    return nullptr;
}
void Renderer::init(SceneObj* parent_)
{
    parent = parent_;
    return ;
}
void Renderer::render(const glm::mat4& mat_){

}