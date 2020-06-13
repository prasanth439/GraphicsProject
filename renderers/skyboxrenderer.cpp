#include "skyboxrenderer.h"
#include "texturegen.h"
SkyBoxRenderer::SkyBoxRenderer(Shader* _shader):Renderer(_shader){
    shader = new Shader("shaders/skyboxvs.glsl","shaders/skyboxfs.glsl");
    std::string skyboxlocs[] ={
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };
    tbo = TextureGen::generateCubeMap(skyboxlocs,6);
    int count_indexes = 0;
    GLfloat vertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    GLuint indexes[] = {
        3,0,1,
        1,2,3,
        4,0,3,
        3,7,4,
        1,5,6,
        6,2,1,
        4,7,6,
        6,5,4,
        3,2,6,
        6,7,3,
        0,4,1,
        1,4,5
    };
    count_indexes = sizeof(indexes)/sizeof(indexes[0]);
    printf("%d count_indexes\n",count_indexes);
    GLuint vbo = 0, ebo = 0;
    // generate and setup vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    // generate and setup element buffer object
    // glGenBuffers(1,&ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexes),indexes,GL_STATIC_DRAW);
    // generate and setup vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    return ;
}

Renderer* SkyBoxRenderer::clone(){
    return new SkyBoxRenderer();
}

void SkyBoxRenderer::render(const glm::mat4& world_mat){
    glDepthFunc(GL_LEQUAL);
    shader->use();
    shader->setMat4("view",glm::mat4(glm::mat3(Camera::main->viewmat)));
    shader->setMat4("project",Camera::main->projmat);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,tbo);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,36);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDepthFunc(GL_LESS);
    return ;
}