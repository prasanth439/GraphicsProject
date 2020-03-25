#include "myrenderer.h"
#include "camera.h"
MyRenderer::MyRenderer()
{
    li = new Light();
    li->position = glm::vec3{1,10,0};
    li->color = glm::vec3{1,1,1};
    model_mat = glm::mat4(1.0f);
    GLfloat vertices[] = {
        -2.5f, -2.5f, -2.5f, -0.5f, -0.5f, -0.5f,// 0
        2.5f, -2.5f, -2.5f,  0.5f, -0.5f, -0.5f, //1
        2.5f,  2.5f, -2.5f,  0.5f,  0.5f, -0.5f, //2
        -2.5f,  2.5f, -2.5f, -0.5f,  0.5f, -0.5f,// 3
        -2.5f, -2.5f,  2.5f, -0.5f, -0.5f,  0.5f,// 4
        2.5f, -2.5f,  2.5f,  0.5f, -0.5f,  0.5f, //5
        2.5f,  2.5f,  2.5f,  0.5f,  0.5f,  0.5f, //6
        -2.5f,  2.5f,  2.5f, -0.5f,  0.5f,  0.5f,// 7
    };

    GLuint indexes[] = {
        0,2,1,//CLK
        2,0,3,//CLK
        4,5,6,
        6,7,4,
        7,3,0,
        0,4,7,
        6,1,2,
        1,6,5,
        0,1,5,
        5,4,0,
        3,6,2,
        6,3,7
    };
    count_indexes = sizeof(indexes)/sizeof(indexes[0]);
    GLuint vbo = 0, ebo = 0;
    int properties_ = 6;
    int vertices_size = 8;
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
    glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, properties_*sizeof(GLuint), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return ;
}

void MyRenderer::render()
{
    Shader* s = this->parent->shader;
    s->use();
    // setting 
    s->setVec3("worldLightPos0",li->position);
    s->setMat4("model",model_mat);
    s->setMat4("view",Camera::main->viewmat);
    s->setMat4("project",Camera::main->projmat);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,count_indexes,GL_UNSIGNED_INT,0);
    return;
}