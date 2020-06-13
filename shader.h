#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include<string>
#include<glm/glm.hpp>
#include "sceneobj.h"

class SceneObj;

class Shader{
    public:
        SceneObj* parent;
        void init(SceneObj* parent_);
        bool use(); 
        void setXform(const GLfloat *mvp); 
        void setMat4(const char* name,const glm::mat4& mvp);
        void setMat4(const char* name,const GLfloat* mvp);
        void setVec2(const char* name,const glm::vec2& vec);
        void setVec3(const char* name,const glm::vec3& vec);
        void setFloat(const char* name,float f);
        Shader(const char *vsfile=NULL, const char *fsfile=NULL);
        ~Shader();
    protected:
        GLuint prog_id;
        GLuint MVPid; // Every shader must have at least an MVP.
        void defaults();
        void makeProgram(GLuint vs, GLint fs);
        virtual void getUniformIDs();
        GLuint vsFromsrc(char *vs_src, bool dofree=true);
        GLuint fsFromsrc(char *fs_src, bool dofree=true);
        void programFromsrc(char *vs_src, char *fs_src, bool dofree=true);
        void programFromfile(const char *vs_file, const char *fs_file);
};


