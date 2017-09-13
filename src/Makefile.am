-include $(top_srcdir)/git.mk


bin_PROGRAMS=main

main_SOURCES=HelloCube.cpp

main_CFLAGS=`pkg-config --cflags glfw3` `pkg-config --cflags glew` `pkg-config --cflags glut`

main_LDFLAGS=`pkg-config --libs glfw3` `pkg-config --libs glew` -lglut


