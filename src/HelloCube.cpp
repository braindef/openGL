#include<iostream>
#include<stdlib.h>

#include<GL/glut.h>

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


void rotatingTriangle(float x, float y, float z)
{
    glPushMatrix(); //Save the transformations performed thus far
    glTranslatef(x,y,z);
    glRotatef(_angle, -1.5f, 0.5f, -5.0f); //Rotate about the z-axis
    glBegin(GL_TRIANGLES);
        glVertex3f(-2.5f, 0.5f, -5.0f);
        glVertex3f(-3.0f, 1.5f, -5.0f);
        glVertex3f(-3.5f, 0.5f, -5.0f);
    glEnd();
    glPopMatrix();
}

std::vector<Vertex> CreateCircleArray(float x, float y, float radius, int fragments)
{
     const float PI = 3.1415926f;

     std::vector<Vertex> result;

     float increment = 2.0f * PI / fragments;

     for (float currAngle = 0.0f; currAngle <= 2.0f * PI; currAngle += increment)
     {
         result.push_back(glm::vec3(radius * cos(currAngle) + x, radius * sin(currAngle) + y, 0));
     }

     return result;
}

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    //rotatingTriangle();
    for (int i=0; i<10; i++) {
	for (int j=0; j<10; j++)
    	{
 	    rotatingTriangle((float)i/10,-(float)j/10,0.0f);

	}
    }

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

    glutInitWindowSize(400,400);

    glutCreateWindow("My Triangle");

    initRendering();

    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyPress);

    glutReshapeFunc(handleResize);

    glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop();

    return(0);
}
