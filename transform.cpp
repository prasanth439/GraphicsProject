#include "transform.h"
#include <glm/gtx/euler_angles.hpp>
Transform::Transform():Transform(glm::vec3(0,0,0),glm::quat(1,0,0,0),glm::vec3(1,1,1))
{

}
Transform::Transform(const glm::vec3& _position,const glm::quat& _rotation,const glm::vec3& _scale):scale(_scale),position(_position),rotation(_rotation)
{
    change = false;
    recompute_model_mat();
}
void Transform::setPosition(const glm::vec3& pos_){
    position = pos_;
    change = true;
}
void Transform::init(SceneObj* _parent){
    parent= _parent;
}
Transform* Transform::clone(Transform* original){
    Transform* newTransform = new Transform(original->position,original->rotation,original->scale);
    return newTransform;
}
void Transform::recompute_model_mat(){
    // model_mat = position*rotation*scale;
    model_mat = get_position()*glm::mat4_cast(rotation);
}
glm::mat4 Transform::get_position() const{
    return glm::mat4{{1,0,0,0},{0,1,0,0},{0,0,1,0},{position.x,position.y,position.z,1}};
}
glm::mat4 Transform::get_scale() const{
    return glm::mat4{{scale.x,0,0,0},{0,scale.y,0,0},{0,0,scale.z,0},{0,0,0,1}};
}

void Transform::check_update(){
    if(change){
        recompute_model_mat();
        change = false;
    }
}