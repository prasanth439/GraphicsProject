#include "renderer.h"
#include "computeshader.h"
#include <chrono>
#pragma once

class NoiseRenderer: public Renderer{
    public:
        GLuint vao,tbo;
        std::chrono::_V2::system_clock::time_point start_;
        Light* li;
        int count_indexes;
        ComputeShader* noise_compute;
        NoiseRenderer(Shader* _shader=nullptr);
        void render(const glm::mat4& worldmat);
        Renderer* clone();
        static GLuint LoadNoiseTexture3D(const char* load_file,glm::ivec4 tex_dim);
        GLuint GenerateNoiseTexture2D(const char* save_file);
        static GLuint LoadNoiseTexture2D(const char* load_file);
        GLuint GenerateNoiseTexture3D(const char* save_file);
};
