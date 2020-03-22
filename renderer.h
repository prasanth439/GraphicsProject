#pragma once
#include "sceneobj.h"
#include <glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
class SceneObj;

class Renderer{
    public:
        SceneObj* parent;
        Renderer();
        void init(SceneObj* parent_);
        virtual void render()= 0;

};

class Light{
    public:
        glm::vec3 position;
        glm::vec3 color;
};  