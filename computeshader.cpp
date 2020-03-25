#include "computeshader.h"
#include <fstream>
#include <iostream>

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

bool ComputeShader::use() { 
   glUseProgram(prog_id); return(glGetError() == GL_NO_ERROR); 
}

void ComputeShader::dispatch_shader(GLuint gr_x,GLuint gr_y,GLuint gr_z){
   glDispatchCompute(gr_x,gr_y,gr_z);
}
void ComputeShader::setXform(const GLfloat *mvp) {
    glUniformMatrix4fv(MVPid, 1, GL_FALSE, mvp);
}
void ComputeShader::setMat4(const char* name,const glm::mat4& mvp)
{
   glUniformMatrix4fv(glGetUniformLocation(prog_id,name),1,GL_FALSE,glm::value_ptr(mvp));
}
void ComputeShader::setMat4(const char* name,const GLfloat* mvp)
{
   glUniformMatrix4fv(glGetUniformLocation(prog_id,name),1,GL_FALSE,mvp);
}
void ComputeShader::setVec2(const char* name,const glm::vec2& vec)
{
   glUniform2f(glGetUniformLocation(prog_id,name),vec.x,vec.y);
}
void ComputeShader::setVec3(const char* name,const glm::vec3& vec){
   glUniform3f(glGetUniformLocation(prog_id,name),vec.x,vec.y,vec.z);
}
void ComputeShader::setFloat(const char* name,float f)
{
   glUniform1f(glGetUniformLocation(prog_id,name),f);
}


ComputeShader::ComputeShader(const char *csfile)
{
   if(csfile == NULL) // Both should default or neither
        defaults();
   else
      programFromfile(csfile);
}


GLuint ComputeShader::csFromsrc(char cs_src[], bool dofree)
{
    if(cs_src == NULL) return -1;
    GLuint csid = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(csid, 1, &cs_src, NULL);
    glCompileShader(csid);
    GLuint status;
    if(dofree) delete[](cs_src);
    return csid;
}

void ComputeShader::programFromsrc(char *cs_src, bool dofree)
{
   makeProgram(csFromsrc(cs_src, dofree));
}

void ComputeShader::programFromfile(const char *cs_file)
{
   programFromsrc(filedata(cs_file));
}

void ComputeShader::makeProgram(GLuint cs)
{
    prog_id = glCreateProgram();
    if(cs >= 0 && isCompiled("Compute", cs)) glAttachShader(prog_id, cs);
    glLinkProgram(prog_id);
    if(isLinked(prog_id))
       glUseProgram(prog_id);
}

void ComputeShader::defaults()
{
   char cs_src[] = "\
         #version 410\n \
         layout(location = 0) in vec3 vp; \
         uniform mat4 MVP; \
         void main() { \
            gl_Position =  MVP * vec4(vp,1.0); \
         } \
      ";

   programFromsrc(cs_src);
}

