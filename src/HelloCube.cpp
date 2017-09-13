#include<iostream>
#include<stdlib.h>
#include<vector>
#include<math.h>

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


void rotatingLine(float x, float y, float z)
{
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR,currentColor);
    glPushMatrix(); //Save the transformations performed thus far
    glColor4f(1.0f, 1.0f, 0.0f,0.5f);
    glTranslatef(x,y,z);
    glLineWidth(10.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glRotatef(_angle, 0.0f, 0.0f, -5.0f); //Rotate about the z-axis
    glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, -5.0f);
        glVertex3f(0.0f, 1.0f, -5.0f);

    glEnd();
    glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
    glPopMatrix();
}

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
 	    rotatingTriangle((float)i/10,-(float)j/10,0.0f);
 	    rotatingLine((float)i/2,-(float)j/2,0.0f);

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
