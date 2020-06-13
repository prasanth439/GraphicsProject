#include "noisegen.h"
glm::vec2* NoiseGen::seeds = nullptr;

float NoiseGen::remap(float oldVal,float newLow,float newHigh,float low, float high){
    return  newLow + (newHigh-newLow)*(oldVal-low)/(high-low);
}

float* NoiseGen::createNoiseMap(int width,int height,float scale,float lac,int octaves,float persistance,glm::vec2 offset,float size){
    if(seeds==nullptr){
        seeds = new glm::vec2[octaves];
        srand(0);
        for(int i=0;i<octaves;i++){
            seeds[i] = {glm::linearRand(-1e5,1e5),glm::linearRand(-1e5,1e5)};
        }
    }
    float* noiseVals = new float[height*width];
    float maxVal = 0;
    float minVal = 0;
    float factX = size/(width-1) ,factY = size/(height-1);
    float tempAmp = 1.0f;
    for(int k=0;k<octaves;k++){
        maxVal+=tempAmp;
        minVal-=tempAmp;
        tempAmp*=persistance;
    }
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            float val = 0;
            float amp = 1.0f;
            float freq  = 1.0f;
            glm::vec2 pos;
            for(int k=0;k<octaves;k++){
                pos =  glm::vec2{(factX*i+offset.x)/scale*freq,(factY*j+offset.y)/scale*freq} +  seeds[k];
                val += amp*glm::perlin(pos);
                amp*=persistance;
                freq*=lac;
            }
            noiseVals[j*width+i] = val;
        }

    }
    for(auto a=0;a<height;a++){
        for(auto b=0;b<width;b++){
            noiseVals[a*width+b] = remap(noiseVals[a*width+b],0.0f,1.0f,minVal,maxVal);
        }
    }
    return noiseVals;
}
int NoiseGen::hi(char a){
    switch(a){
        case '0':return 0;
        case '1':return 1;
        case '2':return 2;
        case '3':return 3;
        case '4':return 4;
        case '5':return 5;
        case '6':return 6;
        case '7':return 7;
        case '8':return 8;
        case '9':return 9;
        case 'a':return 10;
        case 'b':return 11;
        case 'c':return 12;
        case 'd':return 13;
        case 'e':return 14;
        case 'f':return 15;
        default:return 0;
    }
}

glm::ivec3 NoiseGen::convert(string colorVal){
    glm::ivec3 newColor(hi(colorVal[0])*16+hi(colorVal[1]),hi(colorVal[2])*16+hi(colorVal[3]),hi(colorVal[4])*16+hi(colorVal[5]));
    return newColor;
}