#pragma once
#include "renderer.h"
#include "shader.h"

class Shader;
class Renderer;

class SceneObj{
    public:
    Renderer* renderer;
    Shader* shader;
    SceneObj();
    SceneObj(Renderer* renderer_,Shader* shader_);
    SceneObj* init();
    void draw_obj();
};