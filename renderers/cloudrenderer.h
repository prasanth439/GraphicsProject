#include "renderer.h"
#include <chrono>
#pragma once

class CloudRenderer: public Renderer{
    public:
        GLuint vao,tbo;
        glm::mat4 model_mat;
        Light* li;
        std::chrono::_V2::system_clock::time_point start_;
        glm::vec3 resol;
        int count_indexes;
        CloudRenderer();
        void render();
};