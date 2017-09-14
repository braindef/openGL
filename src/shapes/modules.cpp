#include<iostream>
#include<stdlib.h>
#include<vector>
#include<math.h>

#include<GL/glut.h>

void rotatingLine(float x, float y, float z, float angle, float size)
{
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR,currentColor);
    glPushMatrix(); //Save the transformations performed thus far
    glColor4f(1.0f, 0.0f, 0.0f,0.8f);
    glTranslatef(x,y,z);
    glLineWidth(10.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    //glTranslatef
    glScalef(size,size,1.0);
    glRotatef(angle, 0.0f, 0.0f, -5.0f); //Rotate about the z-axis
    glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, -5.0f);
        glVertex3f(0.0f, 1.0f, -5.0f);

    glEnd();
    glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
    glPopMatrix();
}

void rotatingTriangle(float x, float y, float z, float angle, float size)
{
    glPushMatrix(); //Save the transformations performed thus far
    glTranslatef(x,y,z);
    glScalef(size, size, 1);
    glRotatef(angle, -0.5f, 0.5f, -5.0f); //Rotate about the z-axis
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.5f, -0.5f, -5.0f);
        glVertex3f(0.5f, -0.5f, -5.0f);
        glVertex3f(0.5f, 0.5f, -5.0f);
    glEnd();
    glPopMatrix();
}


