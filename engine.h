#pragma once
#include <vector>
#include "sceneobj.h"
#include "camera.h"
#include <GLFW/glfw3.h>
 
class Engine{
    private:// fields
        std::vector<SceneObj* > obj_list;
        GLFWwindow* window;
        bool quit_,ctrl_,shift_;
        double last_X,last_Y;
        static Engine* current_engine;
    public:// methods
        int screen_width;
        int screen_height;
        float fov;
        Engine();
        int start();
        static Engine* get_instance(){
            return current_engine;
        }
        int initialize();
        static void handleKeysGLFW(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void handleKeyRelease(int key,double ,double);
        static void handleKeys(int key,double,double);
        static void handleMouseClickGLFW(GLFWwindow*,int ,int,int);
        static void handleMouseDown(int,double,double);
        static void handleMouseUp(int,double,double);
        static void handleResizeGLFW(GLFWwindow*,int,int);
        static void handleMouseMotionGLFW(GLFWwindow* window, double x, double y);
        glm::vec3 arc_vectors(double x,double y);
        void sceneLoop();
        void scenesetup();
};