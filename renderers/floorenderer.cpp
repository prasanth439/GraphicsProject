#include "floorrenderer.h"
#include "texturegen.h"
FloorRenderer::~FloorRenderer(){
    
}
Renderer* FloorRenderer::clone(){
    return new FloorRenderer(vao,tbo,shader);
}
FloorRenderer::FloorRenderer(GLuint vao_,GLuint tbo_,Shader* shader_):Renderer(shader_){
    vao = vao_;
    tbo = tbo_;
}
FloorRenderer::FloorRenderer(Shader* _shader):Renderer(_shader)
{
    shader = new Shader("shaders/floorvs.glsl","shaders/floorfs.glsl");
    std::string fileName = "seemLessTiles.jpg";
    tbo = TextureGen::generateTexture(fileName);
    GLfloat vertices[] = {
        1.0f,  0.0f, -1.0f, 0.0,1.0f
        -1.0f,  0.0f, -1.0f,0.0f,0.0f,
        1.0f,  0.0f,  1.0f, 1.0f,0.0f,
        -1.0f,  0.0f,  1.0f,1.0f,1.0f
    };

    GLuint indexes[] = {
        1,2,0,
        2,1,3
    };
    count_indexes = sizeof(indexes)/sizeof(indexes[0]);
    GLuint vbo = 0, ebo = 0;
    int properties_ = 5;
    int vertices_size = 4;
    int indexes_size = count_indexes;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ebo);
    return ;
}

void FloorRenderer::render(const glm::mat4& world_mat)
{
    shader->use();
    // setting 
    shader->setMat4("model",world_mat);
    shader->setMat4("view",Camera::main->viewmat);
    shader->setMat4("project",Camera::main->projmat);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,count_indexes,GL_UNSIGNED_INT,0);
    return;
}