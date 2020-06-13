#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/noise.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;
class NMGenerator{
public:
    vector<vector<float>> noiseVals;
    static float remap(float oldVal,float newLow,float newHigh,float low, float high){
        return  newLow + (newHigh-newLow)*(oldVal-low)/(high-low);
    }
    void createNoiseMap(int width,int height,float scale,float lac,int octaves,float persistance,glm::vec2 offset,float size){
        glm::vec2* seeds = new glm::vec2[octaves];
        noiseVals = vector<vector<float>>(height,vector<float>(width));
        srand(0);
        for(int i=0;i<octaves;i++){
            seeds[i] = {glm::linearRand(-1e5,1e5),glm::linearRand(-1e5,1e5)};
        }
        
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
                noiseVals[j][i] = val;
            }

        }
        printf("%f %f\n",maxVal,minVal);
        for(auto &a:noiseVals){
            for(auto &b:a){
                b = remap(b,0.0f,1.0f,minVal,maxVal);
            }
        }
    }
    void randomNum(){
        srand(0);
        printf("%d\n",glm::linearRand(0,100));
        printf("%d\n",glm::linearRand(0,100));
        printf("%d\n",glm::linearRand(0,100));
        printf("%d\n",glm::linearRand(0,100));
        printf("%d\n",glm::linearRand(0,100));
        printf("%d\n",glm::linearRand(0,100));
    }
};
struct Color{
    int x;int y;int z;
    Color(int a,int b,int c){
        x =a;
        y =b;
        z= c;
    }
};
ostream& operator<<(ostream& os,const Color& c){
    os<<c.x<<" "<<c.y<<" "<<c.z;
    return os;
}
int hi(char a){
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
Color convert(string colorVal){
    Color newColor(hi(colorVal[0])*16+hi(colorVal[1]),hi(colorVal[2])*16+hi(colorVal[3]),hi(colorVal[4])*16+hi(colorVal[5]));
    return newColor;
}
int main(){
    NMGenerator g,h;
    int width,height,octaves;
    float scale,lac,persistance,size;
    glm::vec2 offset;
    FILE* inpFile = fopen("props","r");
    fscanf(inpFile,"width:%d\nheight:%d\noctaves:%d\nscale:%f\nlac:%f\npersistance:%f\noffset:%f,%f\nsize:%f",&width,&height,&octaves,&scale,&lac,&persistance,&offset.x,&offset.y,&size);
    g.createNoiseMap(width,height,scale,lac,octaves,persistance,offset,size);
    h.createNoiseMap(width,height,scale,lac,octaves,persistance,offset+glm::vec2(1,0)*200.0f,size);
    ofstream outfile1("visual1.ppm");
    ofstream outfile2("visual2.ppm");
    outfile1<<"P3"<<endl;
    outfile1<<g.noiseVals[0].size()<<" "<<g.noiseVals.size()<<endl;
    outfile1<<255<<endl;
    outfile2<<"P3"<<endl;
    outfile2<<g.noiseVals[0].size()<<" "<<g.noiseVals.size()<<endl;
    outfile2<<255<<endl;
    Color l1 = convert("7c3c21");
    Color l2 = convert("ec823a");
    Color l3 = convert("f9c49a");
    Color l4 = convert("e8e4e1");
    Color darkblue{0,0,128};
    for(int i=0;i<height;i++){
        for(int j=0;j<2*width;j++){
            float b = 0;
            ofstream* tOut = nullptr;
            if(j<width){
                b = g.noiseVals[i][j];
                tOut = &outfile1;
            }
            else{
                b = h.noiseVals[i][j-width];
                tOut = &outfile2;
            }
            Color temp(0,0,0);
            if(b<0.41)
                temp = l1;
            else if(b<0.55){
                temp = l2;
            }
            else if(b<0.68){
                temp = l3;
            }
            else{
                temp = l4;
            }
            
            (*tOut)<<temp<<"\t";
        }
        outfile1<<endl;
        outfile2<<endl;
    }
}