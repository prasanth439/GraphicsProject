#include "renderer.h"
#pragma once

class MyRenderer: public Renderer{
    public:
        GLuint vao;
        Light* li;
        glm::mat4 model_mat;
        MyRenderer();
        void render();
    private:
        int count_indexes;
};