#include "engine.h"
#include "myrenderer.h"
#include "cloudrenderer.h"
#include "gridrenderer.h"
#include "noiserenderer.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <glm/gtx/quaternion.hpp>

Engine* Engine::current_engine = nullptr;
Engine::Engine(){
    screen_width = 800;
    screen_height = 600;
    fov = 60;
    quit_ = false;
    ctrl_ = false;
    lshift_ = false;
    rshift_ = false;
    current_engine = this;
}


int Engine::initialize(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(screen_width, screen_height, "Opengl Project", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if ( GLEW_OK != glewInit() ) {
        glfwTerminate();
    }

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, handleKeysGLFW);
    glfwSetFramebufferSizeCallback(window, handleResizeGLFW);
    glfwSetMouseButtonCallback(window, handleMouseClickGLFW);
    glfwSetCursorPosCallback(window, handleMouseMotionGLFW);
    return 0;
}

void Engine::sceneLoop(){
    // while all objects
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    for(auto a:obj_list)
    {
        a->draw_obj();
    }
}


void Engine::scenesetup()
{
        // global light
    Camera::main = new Camera(screen_width,screen_height,fov,0.1f,1000.0f);
    Shader* cubeShader_ = new Shader("shaders/vertex.glsl","shaders/fragment.glsl");
    Renderer* cubeRenderer_ = new MyRenderer();
    SceneObj* cube = (new SceneObj(cubeRenderer_,cubeShader_))->init();
    #ifdef CLOUD
    Shader* cloudShader_ = new Shader("shaders/cloudVs.glsl","shaders/cloudFs4.glsl");
    Renderer* cloudRenderer_ = new CloudRenderer();
    SceneObj* cloudBox = (new SceneObj(cloudRenderer_,cloudShader_))->init();
    #endif
    Shader* gridShader_ = new Shader("shaders/gridVs.glsl","shaders/gridFs.glsl");
    Renderer* gridRenderer_ = new GridRenderer();
    SceneObj* grid = (new SceneObj(gridRenderer_,gridShader_))->init();
    #ifdef TWO_D
    Shader* noiseShader_ = new Shader("shaders/noise2DVs.glsl","shaders/noise2DFs.glsl");
    #else
    Shader* noiseShader_ = new Shader("shaders/noise3DVs.glsl","shaders/noise3DFs.glsl");
    #endif
    #ifdef NOISE
    Renderer* noiseRenderer_ = new NoiseRenderer();
    SceneObj* noise = (new SceneObj(noiseRenderer_,noiseShader_))->init();
    #endif
    obj_list.push_back(grid);
    obj_list.push_back(cube);
    #ifdef CLOUD
    obj_list.push_back(cloudBox);
    #endif
    #ifdef NOISE
    obj_list.push_back(noise);
    #endif 

}
int Engine::start(){
    if(initialize()==-1){
        std::cout<<"Failed initialization"<<std::endl;
        return -1;
    }
    scenesetup();
    while(!glfwWindowShouldClose(window))
    {
        auto start = std::chrono::high_resolution_clock::now();
        sceneLoop();
        glfwSwapBuffers(window);
        glfwWaitEvents();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        char buff[50];
        sprintf(buff,"Frames per sec: %lf",1000.0/duration.count());
        glfwSetWindowTitle(window,buff);
    }

    return 0;
}

// ############## UI Events ##################### //
void Engine::handleResizeGLFW(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
void Engine::handleKeyRelease(int key,double cursorX,double cursorY){
    switch (key)
    {
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        current_engine->ctrl_ = false;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        current_engine->lshift_ = false;
    case GLFW_KEY_RIGHT_SHIFT:
        current_engine->rshift_ = false;
        break;
    default:
        break;
    }
    return ;
}
void Engine::handleKeysGLFW(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_RELEASE)
    {
        double cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        handleKeyRelease(key, cursorX, cursorY);
        return ;
    }
    if (action != GLFW_PRESS)
    {
        return;
    }
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    handleKeys(key, cursorX, cursorY);
    if(current_engine->quit_)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
void Engine::handleKeys(int key,double cursorX,double cursorY){
    switch (key)
    {
    case GLFW_KEY_P:
        std::cout<<"Cam Pos"<<std::endl;
        std::cout<<Camera::main->pos.x<<" "<<Camera::main->pos.y<<" "<<Camera::main->pos.z<< std::endl;
        break;
    case GLFW_KEY_Q:
        current_engine->quit_ = true;
        break;
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        current_engine->ctrl_ = true;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        current_engine->lshift_ = true;
    case GLFW_KEY_RIGHT_SHIFT:
        current_engine->rshift_ = true;
        break;
    default:
        break;
    }
    return ;
}
void Engine::handleMouseDown(int button,double cursorX,double cursorY){
    current_engine->last_X = cursorX;
    current_engine->last_Y = cursorY;
    return ;
}
void Engine::handleMouseUp(int button,double cursorX,double cursorY){
    current_engine->last_X = cursorX;
    current_engine->last_Y = cursorY;
    return ;
}
void Engine::handleMouseClickGLFW(GLFWwindow* window, int button, int action, int mods){
    double cursorX, cursorY;
    if (action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &cursorX, &cursorY);
        handleMouseDown(button, cursorX, cursorY);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwGetCursorPos(window, &cursorX, &cursorY);
        handleMouseUp(button, cursorX, cursorY);
    }
}
glm::vec3 Engine::arc_vectors(double x,double y)
{
    glm::vec3 pt {2*x/screen_width-1,2*y/screen_height-1,0};
    pt.y *=-1;
    float k;
    if((k=glm::dot(pt,pt))<=1)
        pt.z = glm::sqrt(1- k);
    else
        pt = glm::normalize(pt);
    return pt;
}
glm::vec3 rotate_axis(float angle,glm::vec3& point,glm::vec3 axis)
{
   axis*=glm::sin(angle/2);
   glm::quat q = glm::quat(glm::cos(angle/2.0f),axis.x,axis.y,axis.z);
   glm::vec4 temp = glm::mat4_cast(q)*glm::vec4(point,1);
	return glm::vec3{temp.x,temp.y,temp.z};
};
void Engine::handleMouseMotionGLFW(GLFWwindow* window, double x, double y){
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        glm::vec2 cam_mvmt{0.1f*(x-current_engine->last_X),0.1f*(y-current_engine->last_Y)};
        if(current_engine->ctrl_){
            Camera::main->pos = Camera::main->pos - cam_mvmt.y*Camera::main->camForward;
            Camera::main->calcProjViewNative();           
        }
        else if(current_engine->lshift_)
        {
            Camera::main->pos = rotate_axis(glm::radians(10.)*0.5f*cam_mvmt.y,Camera::main->pos,glm::vec3{0,1,0});
            Camera::main->update_axis(true);
        }
        else if(current_engine->rshift_)
        {
            Camera::main->pos = rotate_axis(glm::radians(10.)*0.5f*cam_mvmt.y,Camera::main->pos,Camera::main->camRight);
            Camera::main->update_axis(true);
        }
        else{
            glm::vec3 mvmt = -cam_mvmt.x*Camera::main->camRight + cam_mvmt.y*Camera::main->camUp; // real movement along vectors
            Camera::main->pos = Camera::main->pos + mvmt;
            Camera::main->calcProjViewNative();
        }
        current_engine->last_X = x;
        current_engine->last_Y = y;
    }
}
