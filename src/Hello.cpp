#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <thread>

#include "GL/glew.h"

#include <GL/glut.h>


#include "shapes/modules.hpp"
#include "pixel/drawPixel.hpp"
#include "sound/sdl.hpp"
//#include "fft1/cava.hpp"
#include "3Dshapes/Cube.hpp"
//#include "text/text.hpp"

const int W = 600;
const int H = 400;

extern double f[200];

Cube* c1=new Cube();

using namespace std;
void keyPress(unsigned char key,int x,int y)
{

    switch(key)
    {
       case 27:
            exit(0);
    }
}

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
}


//Called when the window is resized
void handleResize(int w, int h) {
    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
    gluPerspective(45.0,                  //The camera angle
                   (double)w / (double)h, //The width-to-height ratio
                   1.0,                   //The near z clipping coordinate
                   200.0);                //The far z clipping coordinate
}



float _angle = 30.0f;
float _cameraAngle = 0.0f;


void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //ameisenRennen(400,400);
    //punkt(400,400,200,200);
    //glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
    //glRotatef(_angle, 0.0f, 0.01f, -0.0f);
    float scale;

    if(f[2]<0.5f) scale=0.5f;
    else scale=f[2];

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -8.0f);

    c1->cubeArray(-0.0f, 0.0f, 0.0f, _angle, 0.5f, 0.5f);

    c1->cubeArray(-0.0f, -2.0f, 0.0f, _angle, 0.5f, 0.5f);

    c1->cubeArray(-0.0f, 2.0f, 0.0f, _angle, 0.5f, 0.5f);


    //render_text("The Solid Red Fox Jumps Over The Lazy Dog",          0, 0, 0 ,0);
    //horizontalBlur();
    //verticalBlur();
    //glDrawPixels();

    //display();

    glutSwapBuffers();
}




void update(int value) {
    _angle += 1.0f;
    if (_angle > 360) {
        _angle -= 360;
    }

    glutPostRedisplay(); //Tell GLUT that the display has changed

    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);
}


int fft() {
	//mainFFT();
  }


void music() {
  char* c = "/home/marc/Projects/openGL/src/sound/space.mp3";
  play(c);
}

void graphics() {

}

bool init_resources2() {

return true;}


int main(int argc, char* argv[]) {
    thread t1(music);
    thread t2(fft);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(2*W,2*H);
	glutCreateWindow("My Graph");
	initRendering();

#ifndef NOGLEW
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "No support for OpenGL 2.0 found\n");
		return 1;
	}
#endif
	glutKeyboardFunc(keyPress);
        glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0); //Add a timer
	if (init_resources2()) {
		glutDisplayFunc(drawScene);
		glutMainLoop();
	}
	glutMainLoop();
 	t1.join();
	return 0;
}


