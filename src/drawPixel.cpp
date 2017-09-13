

#include <GL/glut.h>
#include <GL/gl.h>
#include <cstdlib>

const unsigned int W = 200;
const unsigned int H = 200;

void display()
{
    //glClearColor( 0, 0, 0, 1 );
    //glClear( GL_COLOR_BUFFER_BIT );

    unsigned int data[H][W][3];
    for( size_t y = 0; y < H; ++y )
    {
        for( size_t x = 0; x < W; ++x )
        {
            data[y][x][0] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][1] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][2] = ( rand() % 256 ) * 256 * 256 * 256;
        }
    }

    glDrawPixels( W, H, GL_RGB, GL_UNSIGNED_INT, data );

    glutSwapBuffers();
}

int xmain( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( W, H );
    glutCreateWindow( "GLUT" );
    glutDisplayFunc( display );
    glutMainLoop();
    return 0;
}
