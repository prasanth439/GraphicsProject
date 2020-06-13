#include "gridrenderer.h"
#include "camera.h"
GridRenderer::~GridRenderer(){
    glDeleteVertexArrays(1,&vao);
}

Renderer* GridRenderer::clone(){
    return new GridRenderer();
}
GridRenderer::GridRenderer(Shader* _shader):Renderer(_shader)
{
    shader = new Shader("shaders/gridVs.glsl","shaders/gridFs.glsl");
    int grid_unit = 5;
    int grid_size = 9;
    grid_size|=1;
    int vertices_size = 4*grid_size-4;
    int indexes_size = 4*grid_size;
    GLfloat* vertices = new GLfloat[3*vertices_size];
    GLuint* indexes = new GLuint[indexes_size];
    glm::vec3 corner = glm::vec3(-grid_unit*(grid_size>>1),0,grid_unit*(grid_size>>1));
    for(int i=0;i<grid_size;i++)
    {
        glm::vec3 temp = corner + float(i)*glm::vec3(grid_unit,0,0);
        vertices[3*i] = temp.x;
        vertices[3*i+1] = temp.y;
        vertices[3*i+2] = temp.z;
        vertices[3*(vertices_size-grid_size) + 3*i] = temp.x;
        vertices[3*(vertices_size-grid_size) + 3*i+1] = temp.y;
        vertices[3*(vertices_size-grid_size) + 3*i+2] = temp.z - grid_unit*(grid_size-1.0f);
    }
    for(int i=0;i<grid_size-2;i++)
    {
        glm::vec3 temp = corner + float(i+1)*glm::vec3(0,0,-grid_unit);
        vertices[3*grid_size+6*i+0] = temp.x;
        vertices[3*grid_size+6*i+1] = temp.y;
        vertices[3*grid_size+6*i+2] = temp.z;
        vertices[3*grid_size+6*i+3] = temp.x + grid_unit*(grid_size-1.0f);
        vertices[3*grid_size+6*i+4] = temp.y + 0;
        vertices[3*grid_size+6*i+5] = temp.z + 0;
    }

    int h1 = 0,h2=grid_size-1,v1=0,v2=grid_size-1+(grid_size-2)*2+1;
    for(int i=0;i<grid_size;i++)
    {
        indexes[4*i] = h2+2*i-1;
        indexes[4*i+1] = h2+2*i;
        indexes[4*i+2] = v1+i;
        indexes[4*i+3] = v2+i;
        if(i==0)
        {
            indexes[4*i] = 0;
        }
        if(i==grid_size-1){
            indexes[4*i] = 4*grid_size-5-(grid_size-1);
            indexes[4*i+1] = 4*grid_size-5;
        }
    }
    count_indexes = sizeof(indexes)/sizeof(indexes[0]);
    GLuint vbo = 0, ebo = 0;
    int properties_ = 3;
    count_indexes = indexes_size;
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
    glBindVertexArray(0);
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ebo);
    delete[] vertices;
    delete[] indexes;
    return ;
}

void GridRenderer::render(const glm::mat4& world_mat)
{
    shader->use();
    // setting 
    shader->setMat4("model",world_mat);
    shader->setMat4("view",Camera::main->viewmat);
    shader->setMat4("project",Camera::main->projmat);
    glBindVertexArray(vao);
    glDrawElements(GL_LINES,count_indexes,GL_UNSIGNED_INT,0);
    return;
}