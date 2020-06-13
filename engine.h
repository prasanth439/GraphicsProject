#pragma once
#include <vector>
#include "sceneobj.h"
#include "camera.h"
#include "runscript.h"
#include <GLFW/glfw3.h>

class Engine{
    private:// fields
        std::vector<SceneObj* > obj_list;
        std::vector<RunScript* > script_list;
    public:// methods
        static Engine* current_engine;
        int screen_width;
        int screen_height;
        float fov;
        Engine(int screen_width_ = 100,int screen_height_ = 100 ,float fov_ = 100 );
        static Engine* get_instance(){
            return current_engine;
        }
        void addObject(SceneObj* o);
        void sceneLoop(float);
        void sceneFlags();
        void scenesetup();
        void scriptInitialize();
        void render_object(SceneObj* obj,const glm::mat4& world_mat);
};
//// DEFINE FLAGS HERE
// #define NOISE
// #define TWO_D
// #define SAVE_TEXTURE
// #define CLOUD