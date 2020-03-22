#include "myrenderer.h"
#include "camera.h"
MyRenderer::MyRenderer()
{
    model_mat = glm::mat4(1.0f);
    GLfloat vertices[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
    };
    GLuint indexes[] = {
        0,1,2
    };
    GLuint vbo = 0, ebo = 0;
    int properties_ = 3;
    int vertices_size = 3;
    int indexes_size =3;
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
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, properties_*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return ;
}

void MyRenderer::render()
{
    Shader* s = this->parent->shader;
    s->use();
    // setting 
    s->setMat4("model",glm::value_ptr(model_mat));
    s->setMat4("view",glm::value_ptr(Camera::main->viewmat));
    s->setMat4("project",glm::value_ptr(Camera::main->projmat));
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    return;
}