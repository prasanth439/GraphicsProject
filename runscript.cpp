#include "runscript.h"
RunScript::RunScript(){

}

RunScript::~RunScript(){

}

void RunScript::init(){

}

void RunScript::update(float time_step){
    
}

void RunScript::attachParent(SceneObj* obj_){
    scene_obj = obj_;
}
float RunScript::clamp(float v,float lo,float hi){
    return (v<lo)?lo:(v>hi)?hi:v;
}