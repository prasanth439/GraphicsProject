#include "renderer.h"
#pragma once

class GridRenderer: public Renderer{
    public:
        GLuint vao;
        GridRenderer(Shader* _shader=nullptr);
        ~GridRenderer();
        Renderer* clone();
        void render(const glm::mat4& world_mat);
    private:
        int count_indexes;
};