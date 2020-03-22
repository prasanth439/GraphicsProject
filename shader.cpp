#include "shader.h"
#include <fstream>

#include <iostream>

void Shader::init(SceneObj* parent_)
{
    parent = parent_;
    return ;
}

bool Shader::use() { glUseProgram(prog_id); return(glGetError() == GL_NO_ERROR); }
void Shader::setXform(const GLfloat *mvp) {
    glUniformMatrix4fv(MVPid, 1, GL_FALSE, mvp);
}
void Shader::setMat4(const char* name,const GLfloat* mvp)
{
    glUniformMatrix4fv(glGetUniformLocation(prog_id,name),1,GL_FALSE,mvp);
}
void Shader::setVec3(const char* name,const glm::vec3& vec){
    glUniform3f(glGetUniformLocation(prog_id,name),vec.x,vec.y,vec.z);
}
void Shader::setFloat(const char* name,float f)
{
    glUniform1f(glGetUniformLocation(prog_id,name),f);
}

static char *filedata(const char* filename)
{
   if(filename == NULL) return NULL;

   FILE *fp = fopen(filename, "r");

   if (fp == NULL) { 
      printf("File Not Found!\n"); 
      return NULL; 
   } 
  
   fseek(fp, 0L, SEEK_END); 
   long int size = ftell(fp); 
   fseek(fp, 0L, SEEK_SET); 
   
   char *ch = new char[size+1];
   if(ch == NULL) return NULL;
   if (size == (int)fread (ch, 1, size, fp)) {
      fclose(fp); 
      ch[size] = '\0';
      return ch;
   }
   fclose(fp);
   delete[] (ch);
   return NULL;
}


Shader::Shader(const char *vsfile, const char *fsfile)
{
   if(vsfile == NULL && fsfile == NULL) // Both should default or neither
      defaults();
   else
      programFromfile(vsfile, fsfile);
}

void Shader::getUniformIDs()
{
   // "MVP" as an arg is expected. Otherwise overwrite
   MVPid = glGetUniformLocation(prog_id, "MVP");
}

static bool isCompiled(const char *prefix, GLuint shader)
{
   if(! glIsShader(shader)) return false;
   GLint status;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
   if(status == GL_TRUE) return true;
   std::cerr << "*** " << prefix << " Shader compilation failed:\n";

   glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
   if(status > 0) {
      char *log = new char[status];
      glGetShaderInfoLog(shader, status, &status, log);
      std::cerr << log << "----\n";
      delete[] log;
   }
   
   return false;
}

static bool isLinked(GLuint program)
{
   if(! glIsProgram(program)) return false;
   GLint status;
   glGetProgramiv(program, GL_LINK_STATUS, &status);
   if(status == GL_TRUE) return true;
   std::cerr << "*** Shader Program linking failed:\n";

   glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status);
   if(status > 0) {
      char *log = new char[status];
      glGetProgramInfoLog(program, status, &status, log);
      std::cout << log << "----\n";
      delete[] log;
   }
   
   return false;
}
GLuint Shader::vsFromsrc(char vs_src[], bool dofree)
{
    if(vs_src == NULL) return -1;
    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsid, 1, &vs_src, NULL);
    glCompileShader(vsid);
   std::cout<<"Vertex shader "<<glGetError()<<std::endl;
   std::cout<<"fs from src"<<std::endl;
    GLuint status;
    if(dofree) delete[](vs_src);
    return vsid;
}

GLuint Shader::fsFromsrc(char fs_src[], bool dofree)
{
   std::cout<<"Frag shader "<<glGetError()<<std::endl;
   if(fs_src == NULL) return -1;
   std::cout<<"Frag shader "<<glGetError()<<std::endl;

   GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);
   std::cout<<"Frag shader "<<glGetError()<<std::endl;

   glShaderSource(fsid, 1, &fs_src, NULL);
   std::cout<<"Frag shader "<<glGetError()<<std::endl;
   glCompileShader(fsid);
   std::cout<<"Frag shader "<<glGetError()<<std::endl;
   std::cout<<"Frag shader "<<glGetError()<<std::endl;
   std::cout<<"fs from src"<<std::endl;
   if(dofree) delete[](fs_src);
   return fsid;
}

void Shader::programFromsrc(char *vs_src, char *fs_src, bool dofree)
{
   makeProgram(vsFromsrc(vs_src, dofree), fsFromsrc(fs_src, dofree));
}

void Shader::programFromfile(const char *vs_file, const char *fs_file)
{
   programFromsrc(filedata(vs_file), filedata(fs_file));
}

void Shader::makeProgram(GLuint vs, GLint fs)
{
    prog_id = glCreateProgram();
    if(vs >= 0 && isCompiled("Vertex", vs)) glAttachShader(prog_id, vs);
    if(fs >= 0 && isCompiled("Fragment", fs)) glAttachShader(prog_id, fs);

    glLinkProgram(prog_id);
    if(isLinked(prog_id))
       glUseProgram(prog_id);

    getUniformIDs();
}

void Shader::defaults()
{
   char vs_src[] = "\
         #version 410\n \
         layout(location = 0) in vec3 vp; \
         uniform mat4 MVP; \
         void main() { \
            gl_Position =  MVP * vec4(vp,1.0); \
         } \
      ";

   char fs_src[] = "\
         #version 410\n \
         out vec4 frag_colur; \
         void main() { \
           frag_color = vec4(0.5, 0.0, 0.5, 0.5); \
         } \
      ";

   programFromsrc(vs_src, fs_src);
}

