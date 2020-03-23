#include "renderer.h"
#pragma once

class CloudRenderer: public Renderer{
    public:
        GLuint vao;
        glm::mat4 model_mat;
        Light* li;
        CloudRenderer();
        void render();
};