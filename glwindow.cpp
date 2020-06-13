#include "glwindow.h"
#include "camera.h"
#include "engine.h"
GLWindow* GLWindow::current_window = nullptr;
GLWindow::GLWindow(){
    screen_width = 640;
    screen_height = 480;
    current_window = this;
    quit_ = false;
    ctrl_ = false;
    lshift_ = false;
    rshift_ = false;
}
int GLWindow::init(){
    
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
        std::cout<<" Failed glfw"<<std::endl;
    }

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, handleKeysGLFW);
    glfwSetFramebufferSizeCallback(window, handleResizeGLFW);
    glfwSetMouseButtonCallback(window, handleMouseClickGLFW);
    glfwSetCursorPosCallback(window, handleMouseMotionGLFW);
    return 0;
}

int GLWindow::start(){
    Engine::current_engine->scenesetup();
    Engine::current_engine->scriptInitialize();
    auto start = std::chrono::high_resolution_clock::now();
    while(!glfwWindowShouldClose(window))
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed_time = (end-start);
        start = end;
        Engine::current_engine->sceneLoop(elapsed_time.count());
        glfwSwapBuffers(window);
        glfwWaitEvents();
        char buff[50];
        sprintf(buff,"Frame time(ms): %lf",1000*elapsed_time.count());
        glfwSetWindowTitle(window,buff);
    }

    return 0;
}
// ############## UI Events ##################### //
void GLWindow::handleResizeGLFW(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
void GLWindow::handleKeyRelease(int key,double cursorX,double cursorY){
    switch (key)
    {
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        current_window->ctrl_ = false;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        current_window->lshift_ = false;
    case GLFW_KEY_RIGHT_SHIFT:
        current_window->rshift_ = false;
        break;
    default:
        break;
    }
    return ;
}
void GLWindow::handleKeysGLFW(GLFWwindow* window, int key, int scancode, int action, int mods){
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
    if(current_window->quit_)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
void GLWindow::handleKeys(int key,double cursorX,double cursorY){
    switch (key)
    {
    case GLFW_KEY_P:
        break;
    case GLFW_KEY_Q:
        current_window->quit_ = true;
        break;
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        current_window->ctrl_ = true;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        current_window->lshift_ = true;
    case GLFW_KEY_RIGHT_SHIFT:
        current_window->rshift_ = true;
        break;
    default:
        break;
    }
    return ;
}
void GLWindow::handleMouseDown(int button,double cursorX,double cursorY){
    current_window->last_X = cursorX;
    current_window->last_Y = cursorY;
    return ;
}
void GLWindow::handleMouseUp(int button,double cursorX,double cursorY){
    current_window->last_X = cursorX;
    current_window->last_Y = cursorY;
    return ;
}
void GLWindow::handleMouseClickGLFW(GLFWwindow* window, int button, int action, int mods){
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

glm::vec3 rotate_axis(float angle,glm::vec3& point,glm::vec3 axis)
{
   axis*=glm::sin(angle/2);
   glm::quat q = glm::quat(glm::cos(angle/2.0f),axis.x,axis.y,axis.z);
   glm::vec4 temp = glm::mat4_cast(q)*glm::vec4(point,1);
	return glm::vec3{temp.x,temp.y,temp.z};
};
void GLWindow::handleMouseMotionGLFW(GLFWwindow* window, double x, double y){
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        glm::vec2 cam_mvmt{0.1f*(x-current_window->last_X),0.1f*(y-current_window->last_Y)};
        /*
       if(current_window->ctrl_){
            Camera::main->pos = Camera::main->pos - cam_mvmt.y*Camera::main->camForward;
            Camera::main->calcProjViewNative();           
        }
        else if(current_window->lshift_)
        {
            Camera::main->pos = rotate_axis(glm::radians(10.)*0.5f*cam_mvmt.y,Camera::main->pos,glm::vec3{0,1,0});
            Camera::main->update_axis(true);
        }
        else if(current_window->rshift_)
        {
            Camera::main->pos = rotate_axis(glm::radians(10.)*0.5f*cam_mvmt.y,Camera::main->pos,Camera::main->camRight);
            Camera::main->update_axis(true);
        }
        else{
            glm::vec3 mvmt = -cam_mvmt.x*Camera::main->camRight + cam_mvmt.y*Camera::main->camUp; // real movement along vectors
            Camera::main->pos = Camera::main->pos + mvmt;
            Camera::main->calcProjViewNative();
        }
        */
        current_window->last_X = x;
        current_window->last_Y = y;
    }
}
