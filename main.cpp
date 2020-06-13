#include <GL/glew.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include "engine.h"
#ifdef GTK_WIN
#include <gtkmm.h>
#include "gtkwindow.h"
#else
#include "glwindow.h"
#endif
// entry point of the code
Camera* Camera::main = nullptr;

int main(int argc,char*argv[])
{
    Engine eng(640,480,60.0f);
    #ifdef GTK_WIN
    auto app = Gtk::Application::create(argc,argv,"Opengl Application");
    GTKWindow win;
    win.set_resizable(false);
    return app->run(win);
    #else
    GLWindow win;
    win.init();
    win.start();
    return 0;
    #endif
}
