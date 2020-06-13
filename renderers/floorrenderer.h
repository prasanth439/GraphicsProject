#include "renderer.h"
#pragma once

class FloorRenderer: public Renderer{
    public:
        GLuint vao,tbo;
        FloorRenderer(Shader* _shader=nullptr);
        ~FloorRenderer();
        FloorRenderer(GLuint vao_,GLuint tbo_,Shader* shader_);
        Renderer* clone();
        void render(const glm::mat4& world_mat);
    private:
        int count_indexes;
        
};