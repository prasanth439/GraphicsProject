#pragma once
#include "sceneobj.h"
class SceneObj;
class GLWindow;
class RunScript{
public:
    RunScript();
    virtual ~RunScript();
    virtual void init();
    virtual void update(float time_step);
    void attachParent(SceneObj* obj_);
    static float clamp(float v,float lo,float hi);
protected:
SceneObj* scene_obj;
};