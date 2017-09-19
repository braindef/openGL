#include<iostream>
#include<stdlib.h>
#include<vector>
#include<math.h>

#include <string.h>

#include<GL/glut.h>

/*
 * Created by SharpDevelop.
 * User: Marc Jr. Landolt
 * Date: 22.08.2006
 * Time: 07::46
 *
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
//nonsense comment
/// <summary>
/// Description of wuerfel.
/// </summary>

/**
 * A model of a Cube, first there are edge points, they are aggreagatet to lines,
 * and the lines are aggregatet to a cube built out of lines
 * @author landev
 */

    /**
     *
     */
    double P00[3] = { 0.0f,  0.0f,  0.0f};			//Eckpunkte
    double P01[3] = {-0.5f, -0.5f, -0.5f};
    double P02[3] = { 0.5f, -0.5f, -0.5f};
    double P03[3] = { 0.5f,  0.5f, -0.5f};
    double P04[3] = {-0.5f,  0.5f, -0.5f};

    double P11[3] = {-0.5f, -0.5f, 0.5f};
    double P12[3] = { 0.5f, -0.5f, 0.5f};
    double P13[3] = { 0.5f,  0.5f, 0.5f};
    double P14[3] = {-0.5f,  0.5f, 0.5f};


double points[8][3];

   void buildPoints() {
   memcpy(points[0], P01, sizeof P01);
   memcpy(points[1], P02, sizeof P02);
   memcpy(points[2], P03, sizeof P03);
   memcpy(points[3], P04, sizeof P04);
   memcpy(points[4], P11, sizeof P11);
   memcpy(points[5], P12, sizeof P12);
   memcpy(points[6], P13, sizeof P13);
   memcpy(points[7], P14, sizeof P14);
   }

double K01[2][3];
double K02[2][3];
double K03[2][3];
double K04[2][3];
double K05[2][3];
double K06[2][3];
double K07[2][3];
double K08[2][3];
double K09[2][3];
double K10[2][3];
double K11[2][3];
double K12[2][3];


void buildEdges() {
    memcpy(K01[0], P01, sizeof P01);
    memcpy(K01[0], P02, sizeof P02);

    memcpy(K02[1], P02, sizeof P02);
    memcpy(K02[1], P03, sizeof P03);

    memcpy(K03[2], P03, sizeof P03);
    memcpy(K03[2], P04, sizeof P04);

    memcpy(K04[3], P04, sizeof P04);
    memcpy(K04[3], P01, sizeof P01);

    memcpy(K05[0], P11, sizeof P11);
    memcpy(K05[1], P12, sizeof P12);

    memcpy(K06[0], P12, sizeof P12);
    memcpy(K06[1], P13, sizeof P13);

    memcpy(K07[0], P13, sizeof P13);
    memcpy(K07[1], P14, sizeof P14);

    memcpy(K08[0], P14, sizeof P14);
    memcpy(K08[1], P11, sizeof P11);

    memcpy(K09[0], P01, sizeof P01);
    memcpy(K09[1], P11, sizeof P11);

    memcpy(K10[0], P02, sizeof P02);
    memcpy(K10[1], P12, sizeof P12);

    memcpy(K11[0], P03, sizeof P03);
    memcpy(K11[1], P13, sizeof P13);

    memcpy(K12[0], P04, sizeof P04);
    memcpy(K12[1], P14, sizeof P14);

}

double W[12][2][3];

void buildCube() {
    memcpy(W[0], K01, sizeof K01);
    memcpy(W[1], K02, sizeof K02);
    memcpy(W[2], K03, sizeof K03);
    memcpy(W[3], K04, sizeof K04);
    memcpy(W[4], K05, sizeof K05);
    memcpy(W[5], K06, sizeof K06);
    memcpy(W[6], K07, sizeof K07);
    memcpy(W[7], K08, sizeof K08);
    memcpy(W[8], K09, sizeof K09);
    memcpy(W[9], K10, sizeof K10);
    memcpy(W[10], K11, sizeof K11);
    memcpy(W[11], K12, sizeof K12);
}


void Cube(float x, float y, float z, float angle, float size)
{
    buildPoints();
    buildEdges();
    buildCube();
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR,currentColor);
    glPushMatrix(); //Save the transformations performed thus far
    glColor4f(1.0f, 0.0f, 0.0f,0.8f);
    glTranslatef(x,y,z);
    glLineWidth(10.0*size);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    //glTranslatef
    glScalef(size,size,1.0);
    glRotatef(angle, 0.0f, 0.0f, -5.0f); //Rotate about the z-axis
    for (int i=0; i<12; i++)
    {
    glBegin(GL_LINES);
        glVertex3f(W[i][0][0], W[i][0][1], W[i][0][2]);
        glVertex3f(W[i][1][0], W[i][1][1], W[i][1][2]);
    glEnd();
    }
    glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
    glPopMatrix();
}
