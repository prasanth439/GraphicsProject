#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "engine.h"
// entry point of the code
Camera* Camera::main = nullptr;

int main(int argc,const char*argv[])
{
    Engine eng;
    eng.start();
    return 0;
}