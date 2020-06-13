#include "terrainrenderer.h"
#include "texturegen.h"
TerrainRenderer::TerrainRenderer(Shader* _shader):Renderer(_shader){
    shader = new Shader("shaders/terrvs.glsl","shaders/terrfs.glsl");
    li = new Light();
    li->position = glm::vec3{1,10,0};
    li->color = glm::vec3{1,1,1};
    width = 100;
    height = 100;
    size = 200;
    width|=1;
    height|=1;
    GLfloat* vertices = new GLfloat[width*height*5];
    glm::vec3 center(-size/2,0,-size/2);
    float invWidth = 1.0f/(width-1),invHeig = 1.0f/(height-1);
    float factX = size*invWidth,factZ = size*invHeig;
    float* noiseMap = NoiseGen::createNoiseMap(200,200,50,2,4,0.5,glm::vec2(0,0),200);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            vertices[5*width*i+5*j+0] = center.x + j*factX; 
            vertices[5*width*i+5*j+1] = 0;//noiseMap[i*width+j]*20; 
            vertices[5*width*i+5*j+2] = center.z + i*factZ;
            vertices[5*width*i+5*j+3] = j*invWidth; 
            vertices[5*width*i+5*j+4] = i*invHeig; 
        }
    }
    count_indexes = (width-1)*(height-1)*6;
    GLuint* indexes = new GLuint[count_indexes];
    int offset = 0;
    for(int i=0;i<height-1;i++){
        for(int j=0;j<width-1;j++){
            indexes[offset] = width*i+j;
            indexes[offset+1] = width*i+j+1;
            indexes[offset+2] = width*(i+1)+j;

            indexes[offset+3] = width*(i+1)+j;
            indexes[offset+4] = width*i+ j+1;
            indexes[offset+5] = width*(i+1) +j+1;
            offset+=6;
        }
    }
    int properties_ = 5;
    int vertices_size = width*height;
    GLuint vbo = 0, ebo = 0;
    // generate and setup vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, properties_*vertices_size*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    // generate and setup element buffer object
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,count_indexes*sizeof(GLuint),indexes,GL_STATIC_DRAW);
    // generate and setup vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ebo);
}
Renderer* TerrainRenderer::clone(){
    return new TerrainRenderer();
}
void TerrainRenderer::generateMap(const glm::vec2& offset){
    float* noiseMap = NoiseGen::createNoiseMap(200,200,50,2,4,0.5,offset,200);
    tbo = TextureGen::generateTexture(200,200,noiseMap);
}
void TerrainRenderer::render(const glm::mat4& world_mat){
    glBindTexture(GL_TEXTURE_2D,tbo);
    shader->use();
    shader->setVec3("worldLightDir",glm::normalize(li->position));
    shader->setMat4("model",world_mat);
    shader->setMat4("view",Camera::main->viewmat);
    shader->setMat4("project",Camera::main->projmat);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,count_indexes,GL_UNSIGNED_INT,0);
}