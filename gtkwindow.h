#pragma once
#include <gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/paned.h>
#include <gtkmm-3.0/gtkmm/frame.h>
#include <gtkmm/glarea.h>
class GTKWindow : public Gtk::Window {
public:
    Gtk::GLArea oglWindow;
    Gtk::Paned pane_;
    Gtk::Frame uframe;
    Gtk::Frame oframe;
    GTKWindow();
    void onStart();
    void onStop();
    bool onDraw(const Glib::RefPtr<Gdk::GLContext>& context);
};