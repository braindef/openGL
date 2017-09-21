-include $(top_srcdir)/git.mk


bin_PROGRAMS=main

main_SOURCES=Hello.cpp shapes/modules.cpp pixel/drawPixel.cpp sound/sdl.cpp fft1/cava.cpp 3Dshapes/Cube.cpp text/text.cpp

main_CFLAGS=-I/usr/include/freetype2 `pkg-config --cflags gl` `pkg-config --cflags glu`
CPPFLAGS=-I/usr/include/freetype2 `pkg-config --cflags gl` `pkg-config --cflags glu`

main_LDFLAGS=-lglut `pkg-config --libs gl` `pkg-config --libs glu` `pkg-config --libs fftw3` -lSDL -lSDL_mixer -lpthread -lasound -lm -lGLEW -lGL -lfreetype



