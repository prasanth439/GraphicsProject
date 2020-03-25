#include "renderer.h"
#pragma once

class GridRenderer: public Renderer{
    public:
        GLuint vao;
        glm::mat4 model_mat;
        GridRenderer();
        void render();
    private:
        int count_indexes;
};