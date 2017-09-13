-include $(top_srcdir)/git.mk


bin_PROGRAMS=main

main_SOURCES=HelloCube.cpp modules.cpp drawPixel.cpp

main_CFLAGS=`pkg-config --cflags glut` `pkg-config --cflags gl` `pkg-config --cflags glu`

main_LDFLAGS=-lglut `pkg-config --libs gl` `pkg-config --libs glu`


