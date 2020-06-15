#include "texturegen.h"
#include "stb_image.h"

glm::ivec3 TextureGen::colorLevels(float b){
    static glm::ivec3 l1 = NoiseGen::convert("7c3c21");
    static glm::ivec3 l2 = NoiseGen::convert("ec823a");
    static glm::ivec3 l3 = NoiseGen::convert("f9c49a");
    static glm::ivec3 l4 = NoiseGen::convert("e8e4e1");
    if(b<0.41)
        return l1;
    else if(b<0.55){
        return l2;
    }
    else if(b<0.68){
        return l3;
    }
    else{
        return l4;
    }
}
FILE* fp = fopen("visual1.ppm","w");
void TextureGen::createColorMap(GLubyte* dest,const float* source,int width,int height){
    fprintf(fp,"P3\n%d %d %d\n",width,height,255);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            glm::ivec3 v = colorLevels(source[width*i+j]);
            fprintf(fp,"%d %d %d\t",v.x,v.y,v.z);
            dest[3*width*i+3*j+0] = v.x;
            dest[3*width*i+3*j+1] = v.y;
            dest[3*width*i+3*j+2] = v.z;
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    return;
}
GLuint TextureGen::generateTexture(int width,int height,const float* noiseMap){
    GLuint tbo = 0;
    glGenTextures(1,&tbo);
    glBindTexture(GL_TEXTURE_2D,tbo);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
    GLubyte* data = new GLubyte[width*height*3];
    createColorMap(data,noiseMap,width,height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    delete[] data;
    return tbo; 
}
GLuint TextureGen::generateTexture(string& fileName){
    GLuint tbo = 0;
    glGenTextures(1,&tbo);
    glBindTexture(GL_TEXTURE_2D,tbo);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    int width,height,channels;
    GLubyte* data = stbi_load(fileName.c_str(),&width,&height,&channels,0);
    assert(data!=nullptr); 
    printf("Width %d%d %d\n",width,height,channels);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    delete[] data;
    glBindTexture(GL_TEXTURE_2D,0);
    return tbo;
}
GLuint TextureGen::generateCubeMap(string* faces,int num_faces){
    GLuint tbo = 0;
    glGenTextures(1,&tbo);
    glBindTexture(GL_TEXTURE_CUBE_MAP,tbo);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    int width = 0,height = 0,channels = 0;
    for(int i=0;i<num_faces;i++){
        GLubyte* data = stbi_load(faces[i].c_str(),&width,&height,&channels,0);
        assert(data!=nullptr);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        delete[] data;
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    return tbo;
}