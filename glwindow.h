#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
class GLWindow{
public:
    GLFWwindow* window;
    static GLWindow* current_window;
    int screen_width,screen_height;
    float fov;
    bool quit_,ctrl_,lshift_,rshift_;
    double last_X,last_Y;
    GLWindow();
    int init();
    int start();
    static void handleKeysGLFW(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void handleKeyRelease(int key,double ,double);
    static void handleKeys(int key,double,double);
    static void handleMouseClickGLFW(GLFWwindow*,int ,int,int);
    static void handleMouseDown(int,double,double);
    static void handleMouseUp(int,double,double);
    static void handleResizeGLFW(GLFWwindow*,int,int);
    static void handleMouseMotionGLFW(GLFWwindow* window, double x, double y);
};