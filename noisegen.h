#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#pragma once

using namespace std;
class NoiseGen{
public:
    NoiseGen();
    static glm::vec2* seeds;
    static float remap(float oldVal,float newLow,float newHigh,float low, float high);
    static float* createNoiseMap(int width,int height,float scale,float lac,int octaves,float persistance,glm::vec2 offset,float size);
    static int hi(char a);
    static glm::ivec3 convert(string colorVal);
};
