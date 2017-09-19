#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <thread>

#include<GL/glut.h>

#include "shapes/modules.hpp"
#include "pixel/drawPixel.hpp"
#include "sound/sdl.hpp"
#include "fft1/cava.hpp"
#include "3Dshapes/Cube.hpp"

const int W = 400;
const int H = 400;

extern double f[200];

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

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glRotatef(_angle, 0.0f, 0.0f, -5.0f);
    glScalef(0.5,0.5,1);
    for (int i=-5; i<6; i++) {
    	for (int j=-5; j<6; j++)
    	{
 	    Cube((float)i/2,-(float)j/2, -5.0f,f[7],f[2]);

 	    //rotatingLine((float)i/2,-(float)j/2,-5.0f, (f[(i+5)+(j+5)])*180,f[2]);

	}
    }





    //horizontalBlur();
    //verticalBlur();

    //glDrawPixels();

    glutSwapBuffers();
}




void update(int value) {
    _angle += 2.0f;
    if (_angle > 360) {
        _angle -= 360;
    }

    glutPostRedisplay(); //Tell GLUT that the display has changed

    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);
}


int fft() {
	mainFFT();
  }


void music() {
  char* c = "/home/marc/Projects/openGL/src/sound/space.mp3";
  play(c);
}

void graphics() {




}


int main(int argc,char** argv)
{
    thread t1(music);
    thread t2(fft);


    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

    glutInitWindowSize(2*W,2*H);

    glutCreateWindow("My Triangle");

    initRendering();

    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyPress);

    glutReshapeFunc(handleResize);

    glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop();

    t1.join();
    //t2.join();

    return(0);
}
