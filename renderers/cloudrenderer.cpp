#include "cloudrenderer.h"
#include "noiserenderer.h"
#include "camera.h"
#include "engine.h"

CloudRenderer::CloudRenderer()
{
    li = new Light();
    li->position = glm::normalize(glm::vec3{1,10,3});
    li->color = glm::vec3{0,1,0};
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
    tbo = NoiseRenderer::LoadNoiseTexture3D("worleynoise3D.bin");
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
}

void CloudRenderer::render()
{
    glBindTexture(GL_TEXTURE_3D,tbo);
    Shader* s = this->parent->shader;
    s->use();
    // setting 
    glm::mat4 transl = glm::translate(glm::mat4(1),glm::vec3(0,10,0));
    glm::mat4 scale = glm::scale(glm::mat4(1),glm::vec3(3,1,3));
    glm::mat4 temp_model = transl*scale*model_mat;
    Engine* inst_ = Engine::get_instance();
    glm::vec2 screen_resol_ = glm::vec2(inst_->screen_width,inst_->screen_height);
    s->setFloat("focal_len",1/glm::tan(glm::radians(inst_->fov)/2));
    s->setFloat("lightAbsorptionThroughCloud",0.4);
    s->setFloat("lightAbsorptionTowardsSun",0.4);
    s->setFloat("light_steps",4.0f);
    s->setFloat("darknessTh",0.7f);
    s->setVec2("screen_resolution",screen_resol_);
    s->setVec3("phaseParams",glm::vec4(0.6f,0.6f,0.6f,0.6f));
    s->setVec3("boundBoxMax_",glm::vec3(2.5,2.5,2.5));
    s->setVec3("boundBoxMin_",glm::vec3(-2.5,-2.5,-2.5));
    s->setVec3("worldCamPos",Camera::main->pos);
    s->setVec3("worldLightPos0",li->position);
    s->setVec3("LightCol0",li->color);
    s->setMat4("model",temp_model);
    s->setMat4("view",Camera::main->viewmat);
    s->setMat4("project",Camera::main->projmat);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,count_indexes,GL_UNSIGNED_INT,0);
    return;
}