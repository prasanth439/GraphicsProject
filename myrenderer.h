#include "renderer.h"
#pragma once

class MyRenderer: public Renderer{
    public:
        GLuint vao;
        glm::mat4 model_mat;
        MyRenderer();
        void render();
};