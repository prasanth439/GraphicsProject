#include "camera.h"
#include "transform.h"
#include<iostream>
Camera::Camera(int width, int height, float fov, float near, float far)
{
   projection(width, height, fov, near, far); 
}

void Camera::projection(int width, int height, float fovd, float near, float far, bool update) 
{
   projmat = glm::perspective(glm::radians(fovd), (float) width / (float)height, near, far);
}

void Camera::view(Transform* trr){

   glm::quat orient = glm::conjugate(trr->rotation);
   glm::mat4 orientMat = glm::mat4_cast(orient);
   glm::mat4 transMat = glm::translate(glm::mat4(1.0f),-trr->position);
   viewmat = orientMat*transMat;
}
glm::vec3 Camera::pos() const{
   return scene_obj->transform->position;
}
void Camera::calcPVMat(){
   pvmat = projmat*viewmat;
}
void Camera::setParent(SceneObj* p_){
   scene_obj = p_;
}
void Camera::trackChanges(){
   if(scene_obj->transform->change){
      view(scene_obj->transform);
      calcPVMat();
   }
}