#include "GL/glew.h"
#pragma once
#include "noisegen.h"

class TextureGen{
public:
    static glm::ivec3 colorLevels(float b);
    static void createColorMap(GLubyte* dest,const float* source,int width,int height);
    static GLuint generateTexture(int ,int ,const float*);
    static GLuint generateCubeMap(string* faces,int num_faces);
    static GLuint generateTexture(string& fileName);
};