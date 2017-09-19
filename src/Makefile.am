-include $(top_srcdir)/git.mk


bin_PROGRAMS=main

main_SOURCES=Hello.cpp shapes/modules.cpp pixel/drawPixel.cpp sound/sdl.cpp fft1/cava.cpp 3Dshapes/Cube.cpp

main_CFLAGS=`pkg-config --cflags glut` `pkg-config --cflags gl` `pkg-config --cflags glu` -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT

main_LDFLAGS=-lglut `pkg-config --libs gl` `pkg-config --libs glu` `pkg-config --libs fftw3` -lSDL -lSDL_mixer -lpthread -lasound -lm -lfftw3


