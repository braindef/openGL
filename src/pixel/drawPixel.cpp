

#include <GL/glut.h>
#include <GL/gl.h>
#include <cstdlib>



void ameisenRennen( int w,  int h) {
    unsigned int data[w][h][3];
    for( size_t y = 0; y < w; ++y )
    {
        for( size_t x = 0; x < h; ++x )
        {
            data[y][x][0] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][1] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][2] = ( rand() % 256 ) * 256 * 256 * 256;
        }
    }
    glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_INT, data );
}

void punkt( int w,  int h, int x, int y) {

	int data[w][h][3];
            data[y][x][0] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][1] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][2] = ( rand() % 256 ) * 256 * 256 * 256;

    glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_INT, data );
}

