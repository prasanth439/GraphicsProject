#include "engine.h"
#include "myrenderer.h"
#include "cloudrenderer.h"
#include "gridrenderer.h"
#include "noiserenderer.h"
#include "skyboxrenderer.h"
#include "terrainrenderer.h"
#include "floorrenderer.h"
#include "camera.h"
#include "scripts/flightControl.h"
#include "scripts/terrain.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <glm/gtx/quaternion.hpp>
#include <stack>
Engine* Engine::current_engine = nullptr;
// window,camera parameters
Engine::Engine(int screen_width_,int screen_height_ ,float fov_ ){
    current_engine = this;
    screen_height = screen_height_;
    screen_width = screen_width_;
    fov = fov_;
}

// Default scene flags to clear depth color bits
void Engine::sceneFlags(){
    glClearColor(0.2,0.2,0.2,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

}

void Engine::render_object(SceneObj* obj,const glm::mat4& world_mat){
    const glm::mat4 current_world_mat = world_mat*(obj->transform->model_mat);
    // to update the transform
    obj->transform->check_update();
    obj->draw_obj(current_world_mat*(obj->transform->get_scale()));
    for(auto a:obj->children)
    {
        render_object(a,current_world_mat);
    }
}

void Engine::sceneLoop(float time_step){

    // update the scripts
    for(auto a: script_list){
        if(a!=nullptr)
            a->update(time_step);
    }
    
    Camera::main->trackChanges();
    sceneFlags();
    const glm::mat4 identity(1);
    for(auto a: obj_list){
        if((a!=nullptr)&&(a->shouldDisplay))
            render_object(a,identity);
    }
    
}
void Engine::addObject(SceneObj* o){
    obj_list.push_back(o);
}
void Engine::scriptInitialize(){
    for(auto a:script_list){
        if(a!=nullptr)
            a->init();
    }
}

void Engine::scenesetup()
{
        // global light
    Camera::main = new Camera(screen_width,screen_height,fov,0.1f,1000.0f);


    Renderer* cubeRenderer_ = new MyRenderer();
    Transform* cubeTransform_ = new Transform(glm::vec3(0,10,0),glm::quat(1,0,0,0),glm::vec3(1,1,1));
    SceneObj* cube = (new SceneObj(cubeRenderer_,cubeTransform_))->init();

    Renderer* wingRendererL_ = new MyRenderer();
    Transform* wingTransformL_ = new Transform(glm::vec3(-7.6,0,0),glm::quat(1,0,0,0),glm::vec3(2,0.5,1));
    SceneObj* wingL = (new SceneObj(wingRendererL_,wingTransformL_))->init();

    Renderer* wingRendererR_ = new MyRenderer();
    Transform* wingTransformR_ = new Transform(glm::vec3(7.6,0,0),glm::quat(1,0,0,0),glm::vec3(2,0.5,1));
    SceneObj* wingR = (new SceneObj(wingRendererR_,wingTransformR_))->init();


    cube->children.push_back(wingL);
    cube->children.push_back(wingR);
     
    Renderer* nullRenderer_ = new Renderer(nullptr);
    Transform* camTransform_ = new Transform(glm::vec3(38.9947, 19.833, 8.33949),glm::quat(0.737596,-0.1792, 0.59353, 0.132156),glm::vec3(1,1,1));
    SceneObj* camObj = (new SceneObj(nullRenderer_,camTransform_))->init();
    #ifdef CLOUD
    Renderer* cloudRenderer_ = new CloudRenderer(cloudShader_);
    SceneObj* cloudBox = (new SceneObj(cloudRenderer_))->init();
    #endif
    Renderer* gridRenderer_ = new GridRenderer();
    Transform* gridTransform_ = new Transform(glm::vec3(0,10,0),glm::quat(1,0,0,0),glm::vec3(1,1,1));
    SceneObj* grid = (new SceneObj(gridRenderer_,gridTransform_))->init();
    
    Renderer* gridParentRenderer_ = new FloorRenderer();
    Transform* gridParentTransform_ = new Transform(glm::vec3(0,1,0),glm::quat(1,0,0,0),glm::vec3(1,1,1));
    SceneObj* gridParent = (new SceneObj(gridParentRenderer_,gridParentTransform_))->init();
    
    
    SkyBoxRenderer* skyrenderer_  = new SkyBoxRenderer();
    Transform* skytransform_ = new Transform(glm::vec3(0,0,0),glm::quat(1,0,0,0),glm::vec3(1,1,1));
    SceneObj* skybox = (new SceneObj(skyrenderer_,skytransform_))->init();
    #ifdef TWO_D
    Shader* noiseShader_ = new Shader("shaders/noise2DVs.glsl","shaders/noise2DFs.glsl");
    #else
    Shader* noiseShader_ = new Shader("shaders/noise3DVs.glsl","shaders/noise3DFs.glsl");
    #endif
    #ifdef NOISE
    Renderer* noiseRenderer_ = new NoiseRenderer();
    SceneObj* noise = (new SceneObj(noiseRenderer_,noiseShader_))->init();
    #endif

    // lets have run scripts and 
    addObject(grid);
    addObject(cube);
    addObject(camObj);
    addObject(gridParent);
    addObject(skybox);
    #ifdef CLOUD
    addObject(cloudBox);
    #endif
    #ifdef NOISE
    addObject(noise);
    #endif 
    Camera::main->setParent(camObj);
    Camera::main->view(camObj->transform);
    // Camera::main->viewmat = glm::lookAt(glm::vec3(38.9947, 19.833, 8.33949),glm::vec3(0,0,0),glm::vec3(0,1,0));
    Camera::main->calcPVMat();
    FlightControl* fc_ = new FlightControl();
    fc_->attachParent(cube);
    fc_->camObj = camObj;
    
    Terrain* tm_ = new Terrain();
    tm_->attachParent(cube);
    tm_->gridParent = gridParent;
    tm_->terrain_obj = grid;
    script_list.push_back(fc_);
    script_list.push_back(tm_);
}


