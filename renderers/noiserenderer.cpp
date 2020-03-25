#include "noiserenderer.h"
#include "camera.h"
#include "engine.h"

GLuint generateNoiseTexture()
{
    GLuint tbo = 0;
    glGenTextures(1,&tbo);
    glBindTexture(GL_TEXTURE_2D,tbo);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::ivec2 tex_dim{128,128}; 
    GLubyte* data = new GLubyte[tex_dim.x*tex_dim.y*3];
    for(int i=0;i<tex_dim.y;i++){
        for(int j=0;j<tex_dim.x;j++){
            data[3*tex_dim.x*i + 3*j + 0] = rand()%256;
            data[3*tex_dim.x*i + 3*j + 1] = rand()%256;
            data[3*tex_dim.x*i + 3*j + 2] = rand()%256;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_dim.x, tex_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    delete[] data;
    return tbo;
}
NoiseRenderer::NoiseRenderer()
{
    model_mat = glm::mat4(1.0f);
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0,0.0,//0
        1.0f, -1.0f, 1.0,0.0,//1  
        1.0f, 1.0f, 1.0,1.0,//2
        -1.0f, 1.0f,0.0,1.0 //3
    };

    GLuint indexes[] = {
        0,1,2,//CLK
        0,2,3,//CLK
    };
    count_indexes = sizeof(indexes)/sizeof(indexes[0]);
    GLuint vbo = 0, ebo = 0;
    int properties_ = 4;
    int vertices_size = 4;
    int indexes_size = count_indexes;
    tbo = generateNoiseTexture();

    // generate and setup vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, properties_*vertices_size*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    // generate and setup element buffer object
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexes_size*sizeof(GLuint),indexes,GL_STATIC_DRAW);
    // generate and setup vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void NoiseRenderer::render()
{
    Shader* s = this->parent->shader;
    s->use();
    // setting 
    
    glBindTexture(GL_TEXTURE_2D,tbo);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,count_indexes,GL_UNSIGNED_INT,0);
    return;
}