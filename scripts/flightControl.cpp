#include"flightControl.h"
#include "glwindow.h"
#include<GLFW/glfw3.h>
FlightControl::FlightControl(){
    speed = 20.0f;
    acceleration = 0.1f;
    maxSpeed = 10.0f;

    camOffset = glm::vec3(0, 19.833, 28.33949);
    camOffsetMag = 1*glm::length(camOffset);
    camOffset = glm::normalize(camOffset);
    camUp = glm::vec3(0,1,0);
    camFd = glm::vec3(0,0,1);
    angleSpeed = 1.0f;
    objFd = camFd;
    objUp = camUp;
    objFdAxisRot = glm::quat(1,0,0,0);
    objUpAxisRot = glm::quat(1,0,0,0);
    objRtAxisRot = glm::quat(1,0,0,0);
    originalCamRot = glm::quat(1,0,0,0);
    horizontal = 0.0f;
    vertical = 0.0f;
    elevation = 0.0f;
    actionIncrement = 0.1f;
    vState = Event::Stable;
    hState = Event::Stable;
    eState = Event::Stable;
    
}

FlightControl::~FlightControl(){

}

void FlightControl::init(){
    camObj->transform->position = scene_obj->transform->position + camOffsetMag*camOffset;
    glm::vec3 camRt = glm::cross(camUp,camOffset);
    glm::vec3 desup = glm::cross(camOffset,camRt);
    glm::quat rot1 = glm::rotation(camFd,camOffset);
    glm::vec3 newUp = rot1*camUp;
    glm::quat rot2 = glm::rotation(newUp,desup);
    originalCamRot = rot2*rot1;
}
// void FlightControl::updateControls(){

// }

void FlightControl::stateMachine(int action,float &result,Event& state){
    // right;
    switch(state){
        case Event::Stable: 
            switch(action){
                case 0:result = 0.0f;
                    state = Event::Stable;
                break;
                case 1:result+= actionIncrement;
                    state = Event::UpSupportAction;break;
                case -1:result-= actionIncrement;
                    state = Event::DownSupportAction;break;
                default:break;
            }
        break;
        case Event::UpSupportAction:
            switch(action){
                case 0:result-=actionIncrement;
                    state = Event::UpNoAction;
                break;
                case 1:result = glm::min(1.0f,result+actionIncrement);
                    state = Event::UpSupportAction;
                    break;
                case -1:result-= actionIncrement;
                    state = Event::UpOpposeAction;break;
                default:break;
            }        
        break;
        case Event::UpNoAction:
            switch(action){
                case 0:result-=actionIncrement;
                    if(result<0){
                        state = Event::Stable;
                        result = 0;
                    }
                break;
                case 1:result += actionIncrement;
                    state = Event::UpSupportAction;
                    break;
                case -1:result-= actionIncrement;
                    state = Event::UpOpposeAction;break;
                default:break;
            }         
        break;
        case Event::UpOpposeAction:
            switch(action){
                case 0:result-=actionIncrement;
                    if(result<0){
                        state = Event::Stable;
                        result = 0;
                    }
                    state = Event::UpNoAction;
                break;
                case 1:result += actionIncrement;
                    state = Event::UpSupportAction;
                    break;
                case -1:result-= actionIncrement;
                    state = Event::UpOpposeAction;
                    if(result<0){
                        state= Event::DownSupportAction;
                    }
                    break;
                default:break;
            }         
        break;
        case Event::DownSupportAction:
            switch(action){
                case 0:result+=actionIncrement;
                    state = Event::DownNoAction;
                break;
                case 1:result += actionIncrement;
                    state = Event::DownOpposeAction;
                    break;
                case -1:result= glm::max(result-actionIncrement,-1.0f);
                    break;
                default:break;
            }                
        break;
        case Event::DownNoAction:
            switch(action){
                case 0:result+=actionIncrement;
                    if(result>0){
                        state = Event::Stable;
                        result = 0;
                    }
                break;
                case 1:result += actionIncrement;
                    state = Event::DownOpposeAction;
                    break;
                case -1:result-= actionIncrement;
                    state = Event::DownSupportAction;
                    break;
                default:break;
            }        
        break;
        case Event::DownOpposeAction:
            switch(action){
                case 0:result+=actionIncrement;
                    if(result>0){
                        state = Event::Stable;
                        result = 0;
                    }
                    state = Event::DownNoAction;
                break;
                case 1:result += actionIncrement;
                    state = Event::DownOpposeAction;
                    if(result>0){
                        state = Event::UpSupportAction;
                    }
                    break;
                case -1:result-= actionIncrement;
                    state = Event::DownSupportAction;
                    break;
                default:break;
            }          
        break;
        default:break;
    }
}

void FlightControl::update(float time_step){
    //TODO: Based on the input update the direction and movement of 
    //TODO: the flight
    float forward = 0;
    int right = 0;
    int elevate = 0;
    if(glfwGetKey(GLWindow::current_window->window,GLFW_KEY_W)){
        forward = 1;
        // speed = RunScript::clamp(speed+acceleration,-maxSpeed,maxSpeed);
    }
    if(glfwGetKey(GLWindow::current_window->window,GLFW_KEY_S)){
        forward = -1;
        // speed = RunScript::clamp(speed-acceleration,-maxSpeed,maxSpeed);
    }
    if(glfwGetKey(GLWindow::current_window->window,GLFW_KEY_D)){
        right = 1;
    }
    if(glfwGetKey(GLWindow::current_window->window,GLFW_KEY_A)){
        right = -1;
    }
    if(glfwGetKey(GLWindow::current_window->window,GLFW_KEY_RIGHT_CONTROL)){
        elevate = -1;
    }
    if(glfwGetKey(GLWindow::current_window->window,GLFW_KEY_RIGHT_SHIFT)){
        elevate = 1;
    }
    stateMachine(right,horizontal,hState);
    stateMachine(forward,vertical,vState);
    stateMachine(elevate,elevation,eState);
    objFdAxisRot=glm::angleAxis(-horizontal,objFd);
    glm::quat upchange = glm::angleAxis(-horizontal*angleSpeed*time_step,objUp);
    objUpAxisRot*=upchange;
    objRtAxisRot = glm::angleAxis(elevation,glm::vec3(1,0,0));
    glm::vec3 nobjFd = objUpAxisRot*objRtAxisRot*objFd;
    glm::vec3 ncamOffset =(objUpAxisRot*objRtAxisRot*camOffset);
    scene_obj->transform->position = scene_obj->transform->position - nobjFd*forward*speed*time_step;
    scene_obj->transform->rotation =  objUpAxisRot*objFdAxisRot*objRtAxisRot;
    scene_obj->transform->change = true;
    camObj->transform->position = scene_obj->transform->position + camOffsetMag*ncamOffset;

    camObj->transform->rotation = objUpAxisRot*objRtAxisRot*originalCamRot;
    camObj->transform->change = true;
}
