#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include<string>
#include<glm/glm.hpp>
#include "sceneobj.h"

class SceneObj;

class ComputeShader{
    public:
        bool use(); 
        void setXform(const GLfloat *mvp); 
        void setMat4(const char* name,const glm::mat4& mvp);
        void setMat4(const char* name,const GLfloat* mvp);
        void setVec2(const char* name,const glm::vec2& vec);
        void setVec3(const char* name,const glm::vec3& vec);
        void setFloat(const char* name,float f);
        ComputeShader(const char *com_file=NULL);
        void dispatch_shader(GLuint,GLuint,GLuint);
    protected:
        GLuint prog_id;
        GLuint MVPid; // Every shader must have at least an MVP.
        void defaults();
        void makeProgram(GLuint cs);
        GLuint csFromsrc(char *cs_src, bool dofree=true);
        void programFromsrc(char *vs_src, bool dofree=true);
        void programFromfile(const char *cs_file);
};


