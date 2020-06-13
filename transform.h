#pragma once
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
class SceneObj;
class Transform{
public:
    glm::vec3 position,scale;
    glm::quat rotation;
    glm::mat4 model_mat;      
    Transform();
    Transform(const glm::vec3& _position,const glm::quat& _rotation,const glm::vec3& _scale);
    void init(SceneObj* _parent);
    SceneObj* parent;
    void recompute_model_mat();  
    void check_update();
    void setPosition(const glm::vec3&);
    static Transform* clone(Transform* original);
    glm::mat4 get_position() const;
    glm::mat4 get_scale() const;
    bool change;
};