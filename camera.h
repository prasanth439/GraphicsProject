#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#pragma once

class Camera {

public:
    static Camera* main;
    Camera(int width, int height, float fov, float near, float far);
    void update_axis(bool update_);
    void calcProjView();
    void calcProjViewNative();
    void view(glm::vec3 pos, glm::vec3 at, glm::vec3 up);
    void projection(int width, int height, float fovd, float near, float far, bool update=true) ;
    void position(float x, float y, float z, bool update=true);
    void lookat(float x, float y, float z, bool update=true);
    void upward(float x, float y, float z, bool update=true);
    const glm::mat4 viewmatrix();
    const glm::mat4 pvmatrix();
    const glm::mat4 matrix(const glm::mat4 modelmat) const;
    const glm::mat4 matrix() const;
public:
    glm:: vec3 camUp,camRight,camForward;
    glm::mat4 projmat;
    glm::mat4 viewmat;
    glm::mat4 pvmat;
    glm::vec3 pos, at, up;

};
