#pragma once
#include "runscript.h"
#include <glm/gtx/quaternion.hpp>
enum Event{
    Stable,UpSupportAction,UpNoAction,UpOpposeAction,
    DownSupportAction,DownNoAction,DownOpposeAction
};
class FlightControl:public RunScript{
public:
    SceneObj* camObj;
    float speed,acceleration;
    float maxSpeed;
    FlightControl();
    ~FlightControl();
    void init();
    void update(float time_step);
    void stateMachine(int action,float& result,Event& state);
private:
    glm::vec3 camOffset,camUp,camFd;
    glm::vec3 objFd,objUp;
    glm::quat objFdAxisRot,objUpAxisRot,objRtAxisRot;
    glm::quat originalCamRot;
    float camOffsetMag,angleSpeed;
    float horizontal,vertical,elevation;
    float actionIncrement;
    Event vState,hState,eState;
};


