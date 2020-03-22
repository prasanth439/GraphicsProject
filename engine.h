#pragma once
#include <vector>
#include "sceneobj.h"
#include "camera.h"
#include <GLFW/glfw3.h>
 
class Engine{
    private:// fields
        std::vector<SceneObj* > obj_list;
        int screen_width;
        int screen_height;
        GLFWwindow* window;
        static bool quit_;
    public:// methods
        Engine();
        int start();
        int initialize();
        static void handleKeysGLFW(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void handleKeyRelease(int key,double ,double);
        static void handleKeys(int key,double,double);
        static void handleMouseClickGLFW(GLFWwindow*,int ,int,int);
        static void handleMouseDown(int,double,double);
        static void handleMouseUp(int,double,double);
        static void handleResizeGLFW(GLFWwindow*,int,int);
        static void handleMouseMotionGLFW(GLFWwindow* window, double x, double y);
        void sceneLoop();
        void scenesetup();
};