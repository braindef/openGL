#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef NOGLEW
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <GL/glew.h>
#endif
#include <GL/glut.h>
/* Using GLM for our transformation matrix */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

GLuint program;
GLint attribute_coord;
GLint uniform_tex;
GLint uniform_color;

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

GLuint vbo;

FT_Library ft;
FT_Face face;

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
char* file_read(const char* filename)
{
	FILE* in = fopen(filename, "rb");
	if (in == NULL) return NULL;

	int res_size = BUFSIZ;
	char* res = (char*)malloc(res_size);
	int nb_read_total = 0;

	while (!feof(in) && !ferror(in)) {
		if (nb_read_total + BUFSIZ > res_size) {
			if (res_size > 10*1024*1024) break;
			res_size = res_size * 2;
			res = (char*)realloc(res, res_size);
		}
		char* p_res = res + nb_read_total;
		nb_read_total += fread(p_res, 1, BUFSIZ, in);
	}

	fclose(in);
	res = (char*)realloc(res, nb_read_total + 1);
	res[nb_read_total] = '\0';
	return res;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	fprintf(stderr, "%s", log);
	free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLint create_shader(const char* filename, GLenum type)
{
	const GLchar* source = file_read(filename);
	if (source == NULL) {
		fprintf(stderr, "Error opening %s: ", filename); perror("");
		return 0;
	}
	GLuint res = glCreateShader(type);
	glShaderSource(res, 1, &source, NULL);
	free((void*)source);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		fprintf(stderr, "%s:", filename);
		print_log(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}

int init_resources()
{
	/* Initialize the FreeType2 library */
	if(FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return 1;
	}

	/* Load a font */
	if(FT_New_Face(ft, "/home/marc/Projects/openGL/src/text/FreeSans.ttf", 0, &face)) {
		fprintf(stderr, "Could not open the font\n");
		return 1;
	}

	/* Compile and link the shader program */
	GLint link_ok = GL_FALSE;

	GLuint vs, fs;
	if ((vs = create_shader("/home/marc/Projects/openGL/src/text/text.v.glsl", GL_VERTEX_SHADER))	 == 0) return 0;
	if ((fs = create_shader("/home/marc/Projects/openGL/src/text/text.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		print_log(program);
		return 0;
	}

	const char* attribute_name;
	attribute_name = "coord";
	attribute_coord = glGetAttribLocation(program, attribute_name);
	if (attribute_coord == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	const char* uniform_name;
	uniform_name = "tex";
	uniform_tex = glGetUniformLocation(program, uniform_name);
	if (uniform_tex == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "color";
	uniform_color = glGetUniformLocation(program, uniform_name);
	if (uniform_color == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	// Create the vertex buffer object
	glGenBuffers(1, &vbo);

	return 1;
}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void render_text(const char *text, float x, float y, float sx, float sy) {
	const char *p;
	FT_GlyphSlot g = face->glyph;

	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Loop through all characters */
	for(p = text; *p; p++) {
		/* Try to load and render the character */
		if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{x2,	 -y2	, 0, 0},
			{x2 + w, -y2	, 1, 0},
			{x2,	 -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
}

void display()
{
	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

	glUseProgram(program);

	/* White background */
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Enable blending, necessary for our alpha texture */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat black[4] = {0, 0, 0, 1};
	GLfloat red[4] = {1, 0, 0, 1};
	GLfloat transparent_green[4] = {0, 1, 0, 0.5};

	/* Set font size to 48 pixels */
	FT_Set_Pixel_Sizes(face, 0, 48);

	glUniform4fv(uniform_color, 1, black);

	/* Effects of alignment */
	render_text("The Quick Brown Fox Jumps Over The Lazy Dog",        -1 + 8 * sx, 1 - 50 * sy, sx, sy);
	render_text("The Misaligned Fox Jumps Over The Lazy Dog",         -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

	/* Scaling the texture versus changing the font size */
	render_text("The Small Texture Scaled Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 175 * sy, sx * 0.5, sy * 0.5);
	FT_Set_Pixel_Sizes(face, 0, 24);
	render_text("The Small Font Sized Fox Jumps Over The Lazy Dog",        -1 + 8 * sx, 1 - 200 * sy, sx, sy);
	FT_Set_Pixel_Sizes(face, 0, 48);
	render_text("The Tiny Texture Scaled Fox Jumps Over The Lazy Dog",         -1 + 8 * sx, 1 - 225 * sy, sx * 0.25, sy * 0.25);
	FT_Set_Pixel_Sizes(face, 0, 12);
	render_text("The Tiny Font Sized Fox Jumps Over The Lazy Dog",         -1 + 8 * sx, 1 - 250 * sy, sx, sy);
	FT_Set_Pixel_Sizes(face, 0, 48);

	/* Colors and transparency */
	render_text("The Solid Black Fox Jumps Over The Lazy Dog",        -1 + 8 * sx, 1 - 430 * sy, sx, sy);

	glUniform4fv(uniform_color, 1, red);
	render_text("The Solid Red Fox Jumps Over The Lazy Dog",          -1 + 8 * sx, 1 - 330 * sy, sx, sy);
	render_text("The Solid Red Fox Jumps Over The Lazy Dog",          -1 + 28 * sx, 1 - 450 * sy, sx, sy);

	glUniform4fv(uniform_color, 1, transparent_green);
	render_text("The Transparent Green Fox Jumps Over The Lazy Dog",  -1 + 8 * sx, 1 - 380 * sy, sx, sy);
	render_text("The Transparent Green Fox Jumps Over The Lazy Dog",  -1 + 18 * sx, 1 - 440 * sy, sx, sy);

	glutSwapBuffers();
}

void free_resources()
{
	glDeleteProgram(program);
}

int xyzzyzy(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("My Graph");

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

	if (init_resources()) {
		glutDisplayFunc(display);
		glutMainLoop();
	}

	free_resources();
	return 0;
}

