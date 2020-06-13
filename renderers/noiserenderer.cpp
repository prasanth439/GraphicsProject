#include "noiserenderer.h"
#include "engine.h"


static void save_texture_to_file(void* ptr,uint size_,const char* name){
    FILE* noise_write = fopen(name,"wb");
    fwrite(ptr,size_,1,noise_write);
    return ;   
}

static void load_texture_data_from_file(const char* name,void* data){
    FILE* noise_read  = fopen(name,"rb");
    long int size_ = 0;
    fseek(noise_read,0L,SEEK_END);
    size_ = ftell(noise_read);
    fseek(noise_read,0L,SEEK_SET);
    fread(data,size_,1,noise_read);
    return ;
}

GLuint NoiseRenderer::GenerateNoiseTexture2D(const char* save_file)
{
    glm::ivec2 tex_dim{256,256}; 
    glm::vec2 time_scale{10,30};
    GLuint tbo = 0;

    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_dim.x, tex_dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, tbo, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F); 
    noise_compute = new ComputeShader("shaders/compute/worley2d.glsl");
    noise_compute->use();
    noise_compute->setVec2("time_scale",time_scale);
    noise_compute->dispatch_shader(tex_dim.x/ 16,tex_dim.y/ 16,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    uint buffer_size = tex_dim.x*tex_dim.y*4;
    GLfloat* tex_output = new GLfloat[buffer_size];
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,tex_output);
    #ifdef SAVE_TEXTURE
    save_texture_to_file(tex_output,sizeof(GLfloat)*buffer_size,save_file);
    #endif
    return tbo;
}

GLuint NoiseRenderer::LoadNoiseTexture2D(const char* load_file)
{
    GLuint tbo = 0;
    glGenTextures(1,&tbo);
    glBindTexture(GL_TEXTURE_2D,tbo);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::ivec2 tex_dim{256,256}; 
    GLfloat* data = new GLfloat[tex_dim.x*tex_dim.y*4];
    load_texture_data_from_file(load_file,data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_dim.x, tex_dim.y, 0, GL_RGBA, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    delete[] data;
    return tbo;
}

GLuint NoiseRenderer::GenerateNoiseTexture3D(const char* save_file)
{
    glm::ivec3 tex_dim{256,256,256}; 
    glm::vec2 time_scale{10,30};
    GLuint tbo = 0;

    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_3D, tbo);
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, tex_dim.x, tex_dim.y, tex_dim.z, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, tbo, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F); 
    noise_compute = new ComputeShader("shaders/compute/worley3d.glsl");
    noise_compute->use();
    noise_compute->setVec2("time_scale",time_scale);
    noise_compute->dispatch_shader(tex_dim.x/ 8,tex_dim.y/ 8,tex_dim.z/ 8);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    uint buffer_size = tex_dim.x*tex_dim.y*tex_dim.z*4;
    #ifdef SAVE_TEXTURE
    GLfloat* tex_output = new GLfloat[buffer_size];
    glGetTexImage(GL_TEXTURE_3D,0,GL_RGBA,GL_FLOAT,tex_output);
    save_texture_to_file(tex_output,sizeof(GLfloat)*buffer_size,save_file);
    #endif
    return tbo;
}

GLuint NoiseRenderer::LoadNoiseTexture3D(const char* load_file,glm::ivec4 tex_dim)
{
    GLuint tbo = 0;
    glGenTextures(1,&tbo);
    glBindTexture(GL_TEXTURE_3D,tbo);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    GLfloat* data = new GLfloat[tex_dim.x*tex_dim.y*tex_dim.z*tex_dim.w];
    load_texture_data_from_file(load_file,data);
    glTexImage3D(GL_TEXTURE_3D, 0, (tex_dim.w==3)?GL_RGB32F:GL_RGBA32F, tex_dim.x, tex_dim.y, tex_dim.z , 0,(tex_dim.w==3)?GL_RGB:GL_RGBA, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D,0);
    delete[] data;
    return tbo;
}
Renderer* NoiseRenderer::clone(){
    return new NoiseRenderer();
}
NoiseRenderer::NoiseRenderer(Shader* _shader):Renderer(_shader)
{
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
    #ifdef TWO_D
    tbo = GenerateNoiseTexture2D("worleynoise2D.bin");
    // tbo = LoadNoiseTexture2D("worleynoise2D.bin");
    #else
    tbo = GenerateNoiseTexture3D("worleynoise3D.bin");
    // tbo = LoadNoiseTexture3D("worleynoise3D.bin",glm::ivec4(256,256,256,4));
    #endif

    start_ = std::chrono::high_resolution_clock::now();
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

void NoiseRenderer::render(const glm::mat4& world_mat)
{
    float z_value;
    // setting 
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start_);
    z_value = glm::sin(duration.count()*0.00005);
    // glm::ivec2 tex_dim{256,256}; 
    // glm::vec2 time_scale{(float)duration.count()/100,25};
    #ifdef TWO_D
    // tbo = GenerateNoiseTexture2D();
    glBindTexture(GL_TEXTURE_2D,tbo);
    #else
    glBindTexture(GL_TEXTURE_3D,tbo);
    #endif
    // glActiveTexture(GL_TEXTURE0);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_dim.x, tex_dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
    // glBindImageTexture(0, tbo, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F); 
    // noise_compute->use();
    // noise_compute->setVec2("time_scale",time_scale);
    // noise_compute->dispatch_shader(tex_dim.x/ 16,tex_dim.y/ 16,1);
    // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);  
    shader->use();
    #ifndef TWO_D
    shader->setFloat("z",z_value);
    #endif
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,count_indexes,GL_UNSIGNED_INT,0);
    return;
}