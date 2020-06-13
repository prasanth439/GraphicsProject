#pragma once
#include "glm/gtc/quaternion.hpp"
#include "transform.h"
#include "sceneobj.h"
class Camera {

public:
    static Camera* main;
    // SceneObj* tieup;
    Camera(int width, int height, float fov, float near, float far);
    void projection(int width, int height, float fovd, float near, float far, bool update=true) ;
    void view(Transform*);
    void calcPVMat();
    glm::vec3 pos() const;
    void trackChanges();
    void setParent(SceneObj* p_);
public:
    glm::mat4 projmat;
    glm::mat4 viewmat;
    glm::mat4 pvmat;
private:
    SceneObj* scene_obj;
};
