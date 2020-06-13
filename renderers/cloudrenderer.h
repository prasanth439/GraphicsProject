#include "renderer.h"
#include <chrono>
#pragma once

class CloudRenderer: public Renderer{
    public:
        GLuint vao,tbo;
        Light* li;
        std::chrono::_V2::system_clock::time_point start_;
        glm::vec3 resol;
        int count_indexes;
        CloudRenderer(Shader* _shader=nullptr);
        Renderer* clone();
        void render(const glm::mat4& world_mat);
};