#include "gtkwindow.h"
#include "engine.h"
#include <iostream>

GTKWindow::GTKWindow(){
    pane_ = Gtk::Paned(Gtk::Orientation::ORIENTATION_HORIZONTAL);
    set_title( "OpenGl" ); // Sets the window title.
    set_default_size( 900, 600 ); // Set default size, width and height, in pixels.
    set_border_width( 10 );
    add( pane_ );
    oframe.add(oglWindow);
    pane_.add1( oframe );
    pane_.add2( uframe );
    oframe.set_label( "opengl" );
    oframe.set_shadow_type( Gtk::ShadowType::SHADOW_ETCHED_OUT );
    uframe.set_label( "UI" );
    uframe.set_shadow_type( Gtk::ShadowType::SHADOW_ETCHED_OUT );
    oglWindow.set_auto_render();
    // oglWindow.set_hexpand();
    // oglWindow.set_vexpand();
    oglWindow.set_halign(Gtk::ALIGN_FILL);
    oglWindow.set_valign(Gtk::ALIGN_FILL);
    oglWindow.set_size_request(640, 360);
    oglWindow.set_required_version(4, 2); //your desired gl version

    oglWindow.signal_realize().connect(sigc::mem_fun(this, 
    &GTKWindow::onStart));
    oglWindow.signal_unrealize().connect(sigc::mem_fun(this, 
    &GTKWindow::onStop), false);
    oglWindow.signal_render().connect(sigc::mem_fun(this, 
    &GTKWindow::onDraw));
    oglWindow.add_events(Gdk::KEY_PRESS_MASK| Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON_PRESS_MASK);
    // oglWindow.signal_motion_notify_event().connect(sigc::mem_fun(this,&mainWindow::rotate));
    show_all_children();
}
void GTKWindow::onStart(){
    oglWindow.make_current();
    if(glewInit() != GLEW_OK) {
        std::cout << "Couldnot init GLEW" << std::endl;
    }
    Engine::current_engine->scenesetup();
}
void GTKWindow::onStop(){
    
}
bool GTKWindow::onDraw(const Glib::RefPtr<Gdk::GLContext>& context){
    Engine::current_engine->sceneLoop();
}