#include "cloudrenderer.h"
#include "camera.h"
CloudRenderer::CloudRenderer()
{
    li = new Light();
    li->position = glm::vec3{0,5,0};
    li->color = glm::vec3{1,1,1};
    model_mat = glm::mat4(1.0f);
    GLfloat vertices[] = {
        -1.0f,  -1.0f,
        1.0f,  -1.0f, 
        1.0f,  1.0f, 
        -1.0f,  1.0f
    };
    GLuint indexes[] = {
        0,1,2,
        0,2,3
    };
    GLuint vbo = 0, ebo = 0;
    int properties_ = 2;
    int vertices_size = 4;
    int indexes_size = 6;
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
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return ;
}

void CloudRenderer::render()
{
    Shader* s = this->parent->shader;
    s->use();
    // setting 
    s->setVec3("worldCamPos",Camera::main->pos);
    s->setVec3("worldLightPos0",li->position);
    s->setMat4("model",model_mat);
    s->setMat4("view",Camera::main->viewmat);
    s->setMat4("project",Camera::main->projmat);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    return;
}