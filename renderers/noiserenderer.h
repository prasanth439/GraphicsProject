#include "renderer.h"
#include "computeshader.h"
#include <chrono>
#pragma once

class NoiseRenderer: public Renderer{
    public:
        GLuint vao,tbo;
        glm::mat4 model_mat;
        std::chrono::_V2::system_clock::time_point start_;
        Light* li;
        int count_indexes;
        ComputeShader* noise_compute;
        NoiseRenderer();
        void render();
        static GLuint LoadNoiseTexture3D(const char* load_file);
        GLuint GenerateNoiseTexture2D(const char* save_file);
        static GLuint LoadNoiseTexture2D(const char* load_file);
        GLuint GenerateNoiseTexture3D(const char* save_file);
};
