CC=g++
DEBUG=-g

CFLAGS=$(INCLUDES) $(DEBUG)
INCLUDES=-I . -I renderers
TOPDIR=.
BUILDDIR=$(TOPDIR)/build
BUILDDIRVAR=$(TOPDIR)\/build
DEPFILE=.depend

.PHONY: clean main destroy
main: $(BUILDDIR)/main

depend: $(DEPFILE)

SRCS=engine.cpp main.cpp renderers/myrenderer.cpp renderers/noiserenderer.cpp renderers/cloudrenderer.cpp renderers/gridrenderer.cpp renderer.cpp shader.cpp sceneobj.cpp camera.cpp 
OBJS = $(SRCS:%.cpp=$(BUILDDIR)/%.o)

$(DEPFILE): $(SRCS)
	-rm -f $(DEPFILE)
	$(CC) $(INCLUDES) -MM $^ | sed 's/^\([^ ]*\)\.o/$(BUILDDIRVAR)\/\1.o/' > $(DEPFILE)

include $(DEPFILE)

$(BUILDDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

EXTLIB=-pthread -lglfw -lGLEW -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl

$(BUILDDIR)/main: $(OBJS)
	$(CC) $(DEBUG) -o $@ $^  $(EXTLIB)

clean:
	-rm -f $(OBJS) $(DEPFILE) $(BUILDDIR)/main
