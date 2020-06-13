CC=g++
DEBUG=-g
GTK_FLAG=`pkg-config gtkmm-3.0 --cflags --libs` -DGLIB_DISABLE_DEPRECATION_WARNINGS -DGTK_WIN
WIN_FLAG=gl_win
INCLUDES=-I . -I renderers
TOPDIR=.
BUILDDIR=$(TOPDIR)/build
BUILDDIRVAR=$(TOPDIR)\/build
DEPFILE=.depend
ifeq ($(WIN_FLAG),gtk_win)
CFLAGS=$(INCLUDES) $(DEBUG) $(GTK_FLAG)
SRCS=noisegen.cpp texturegen.cpp scripts/flightControl.cpp scripts/terrain.cpp runscript.cpp transform.cpp engine.cpp gtkwindow.cpp main.cpp renderers/floorenderer.cpp renderers/skyboxrenderer.cpp renderers/terrainrenderer.cpp renderers/myrenderer.cpp renderers/noiserenderer.cpp renderers/cloudrenderer.cpp renderers/gridrenderer.cpp computeshader.cpp renderer.cpp shader.cpp sceneobj.cpp camera.cpp 
else
CFLAGS=$(INCLUDES) $(DEBUG)
SRCS=noisegen.cpp texturegen.cpp scripts/flightControl.cpp scripts/terrain.cpp runscript.cpp transform.cpp engine.cpp glwindow.cpp main.cpp renderers/floorenderer.cpp renderers/skyboxrenderer.cpp renderers/terrainrenderer.cpp renderers/myrenderer.cpp renderers/noiserenderer.cpp renderers/cloudrenderer.cpp renderers/gridrenderer.cpp computeshader.cpp renderer.cpp shader.cpp sceneobj.cpp camera.cpp 
endif
	

.PHONY: clean main destroy
main: $(BUILDDIR)/main

depend: $(DEPFILE)

OBJS = $(SRCS:%.cpp=$(BUILDDIR)/%.o)

$(DEPFILE): $(SRCS)
	-rm -f $(DEPFILE)
	$(CC) $(INCLUDES) -MM $^ | sed 's/^\([^ ]*\)\.o/$(BUILDDIRVAR)\/\1.o/' > $(DEPFILE)

include $(DEPFILE)

$(BUILDDIR)/%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

EXTLIB=-pthread -lglfw -lGLEW -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl

$(BUILDDIR)/main: $(OBJS)
	$(CC) -o $@ $^  $(EXTLIB) $(CFLAGS)

clean:
	-rm -f $(OBJS) $(DEPFILE) $(BUILDDIR)/main
