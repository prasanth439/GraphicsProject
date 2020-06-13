#include "renderer.h"
#pragma once

class TerrainRenderer: public Renderer{
public:
    GLuint vao;
    GLuint tbo;
    TerrainRenderer(Shader* _shader=nullptr);
    Renderer* clone();
    void generateMap(const glm::vec2& offset);
    void render(const glm::mat4&world_mat);
private:
    int count_indexes;
    float size,persistance;
    int width,height,scale,lac,octaves;
    Light* li;
};