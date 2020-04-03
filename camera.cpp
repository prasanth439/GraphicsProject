#include "camera.h"
#include<iostream>
Camera::Camera(int width, int height, float fov, float near, float far)
{
    // projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
    projection(width, height, fov, near, far); 
    position(38.9947, 19.833, 8.33949, false);
    lookat(0.0, 0.0, 0.0, false);
    upward(0.0, 1.0, 0.0, false);
	update_axis(false);
    calcProjView();
    // orientation = glm::quat(up);
}

void Camera::update_axis(bool update_){
	camForward= glm::normalize(pos-at);// cam X
	camRight = glm::normalize(glm::cross(up,camForward));// cam Z
	camUp = glm::cross(camForward,camRight);// cam Y
	if(update_)
		calcProjViewNative();
}
void Camera::calcProjViewNative()
{
   viewmat = glm::mat4(
      glm::vec4(camRight.x,camUp.x,camForward.x,0),
      glm::vec4(camRight.y,camUp.y,camForward.y,0),
      glm::vec4(camRight.z,camUp.z,camForward.z,0),
      glm::vec4(-glm::dot(pos,camRight),-glm::dot(pos,camUp),-glm::dot(pos,camForward),1.0)
   );
   pvmat =  projmat * viewmat;
}
void Camera::calcProjView()
{
   viewmat = glm::lookAt(pos, at, up);
   pvmat =  projmat * viewmat;
}

void Camera::view(glm::vec3 pos, glm::vec3 at, glm::vec3 up)
{
   viewmat = glm::lookAt(pos, at, up);
}

void Camera::projection(int width, int height, float fovd, float near, float far, bool update) 
{
   projmat = glm::perspective(glm::radians(fovd), (float) width / (float)height, near, far);
   if(update) calcProjView();
}

void Camera::position(float x, float y, float z, bool update)
{
   pos = glm::vec3(x, y, z);
   if(update) calcProjView();
}

void Camera::lookat(float x, float y, float z, bool update)
{
   at = glm::vec3(x, y, z);
   if(update) calcProjView();
}

void Camera::upward(float x, float y, float z, bool update)
{
   up = glm::vec3(x, y, z);
   if(update) calcProjView();
}

const glm::mat4 Camera::viewmatrix()
{
   return viewmat; 
}

const glm::mat4 Camera::pvmatrix()
{
   return pvmat;
}

const glm::mat4 Camera::matrix() const
{
    // return glm::mat4(1.0f); // Test identity
    // return projmat * viewmat; // Remember, matrix multiplication is the other way around
    return pvmat; // Remember, matrix multiplication is the other way around
}

const glm::mat4 Camera::matrix(const glm::mat4 modelmat) const
{
    // ModelViewProjection
    return pvmat * modelmat; // Remember, matrix multiplication is the other way around
}
