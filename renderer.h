#pragma once
#include "sceneobj.h"
#include "camera.h"
#include <glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
class SceneObj;
class Shader;
class Renderer{
    public:
        SceneObj* parent;
        Shader* shader;
        Renderer(Shader* _shader);
        virtual ~Renderer();
        void init(SceneObj* parent_);
        virtual Renderer* clone();
        virtual void render(const glm::mat4&);
};

class Light{
    public:
        glm::vec3 position;
        glm::vec3 color;
        Light(){};
        Light(glm::vec3 pos_,glm::vec3 color_):position(pos_),color(color_){}
};  