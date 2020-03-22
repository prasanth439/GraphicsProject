#include "engine.h"
#include "myrenderer.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool Engine::quit_ = false;
Engine::Engine(){
    screen_height = 800;
    screen_width = 600;
    quit_ = false;
}


int Engine::initialize(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", NULL, NULL);

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
    for(auto a:obj_list)
    {
        a->draw_obj();
    }
}


void Engine::scenesetup()
{
    float fov = 60;
    Camera::main = new Camera(screen_width,screen_height,fov,0.1f,100.0f);
    Shader* cubeShader_ = new Shader("shaders/vertex.glsl","shaders/fragment.glsl");
    Renderer* cubeRenderer_ = new MyRenderer();
    SceneObj* cube = (new SceneObj(cubeRenderer_,cubeShader_))->init();
    obj_list.push_back(cube);
    
}
int Engine::start(){
    int init = initialize();
    if(init==-1)
    {
        return -1;
    }
    scenesetup();
    while(!glfwWindowShouldClose(window))
    {
        sceneLoop();
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}

// ############## UI Events ##################### //
void Engine::handleResizeGLFW(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
void Engine::handleKeyRelease(int key,double cursorX,double cursorY){
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
    if(Engine::quit_)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
void Engine::handleKeys(int key,double cursorX,double cursorY){
    switch (key)
    {
    case GLFW_KEY_Q:
        quit_ = true;
        break;
    default:
        break;
    }
    return ;
}
void Engine::handleMouseDown(int button,double cursorX,double cursorY){
    return ;
}
void Engine::handleMouseUp(int button,double cursorX,double cursorY){
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

void Engine::handleMouseMotionGLFW(GLFWwindow* window, double x, double y){
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {

    }
}
