#include<iostream>
#include<stdlib.h>
#include<vector>
#include<math.h>

#include<GL/glut.h>

#include "modules.hpp"


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

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    //rotatingTriangle();
    for (int i=-5; i<5; i++) {
	for (int j=-5; j<5; j++)
    	{
 	    rotatingTriangle((float)i/2,-(float)j/2,0.0f, _angle);
 	    rotatingLine((float)i/2,-(float)j/2,0.0f, _angle);

	}
    }

    unsigned int W=400;
    unsigned int H=400;

    unsigned int data[W][H][3];
    for( size_t y = 0; y < W; ++y )
    {
        for( size_t x = 0; x < H; ++x )
        {
            data[y][x][0] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][1] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][2] = ( rand() % 256 ) * 256 * 256 * 256;
        }
    }

    glDrawPixels( W, H, GL_RGB, GL_UNSIGNED_INT, data );

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

int main(int argc,char** argv)
{
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

    glutInitWindowSize(800,800);

    glutCreateWindow("My Triangle");

    initRendering();

    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyPress);

    glutReshapeFunc(handleResize);

    glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop();

    return(0);
}
