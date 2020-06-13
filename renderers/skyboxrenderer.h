#include "renderer.h"
#pragma once

class SkyBoxRenderer: public Renderer{
    public:
        GLuint vao,tbo;
        SkyBoxRenderer(Shader* _shader=nullptr);
        Renderer* clone();
        void render(const glm::mat4& world_mat);
    private:
        int count_indexes;
};