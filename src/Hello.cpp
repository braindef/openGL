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
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
    //glRotatef(_angle, 0.0f, 0.01f, -0.0f);
    float scale;

    if(f[2]<0.5f) scale=0.5f;
    else scale=f[2];

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f+f[1]);

    c1->cubeArray(-3.0f, 0.0f, 0.0f, _angle, 0.5f, 0.5f);

    c1->cubeArray(-3.0f, -2.0f, 0.0f, _angle+10, 0.5f, 0.5f);

    c1->cubeArray(-3.0f, 2.0f, 0.0f, _angle+20, 0.5f, 0.5f);
    //horizontalBlur();
    //verticalBlur();
    //glDrawPixels();
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
