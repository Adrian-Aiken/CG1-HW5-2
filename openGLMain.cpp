//
//  tessMain.cpp
//  
//
//  Main program for tessellation assignment.
//
//  Students should not be modifying this file.
//

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>

#ifdef __APPLE__ 
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif
#include <SOIL.h>

#include "shaderProgram.h"
#include "simpleShape.h"
#include "cg1Shape.h"

#define CUBE 0
#define CYLINDER 1
#define CONE 2
#define SPHERE 3
#define BG 4

int division1 = 6;
int division2 = 6;
int counter = 0;
float timePassed = 0.0;

void cube();

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define ALL_THE_DAMN_SHAPES (vector<shape>::iterator it = shapes.begin(); it != shapes.end(); it++)

GLuint program;

// Uniform Locations
GLuint theta, trans, scale;
GLuint cPosition, cLookAt, cUp;
GLuint gLeft, gRight, gTop, gBottom, gNear, gFar;
GLuint doOrtho;
GLuint lightPosition;
GLuint ambient, diffuse, specular, specexp;
GLuint outline, sun;

// Background stuff
GLuint bg, ebg, nbg, uvbg;

// Global Uniform Values
float cameraPosition[3] = {0.0, 0.0, 1.0};
float lookAt[3] = {0.0, 0.0, 0.0};
float up[3] = {0.0, 1.0, 0.0};

float fLeft = -3.0;
float fRight = 3.0;
float fTop = 3.0;
float fBottom = -3.0;
float fNear = -5.0;
float fFar = 30.0;

float ortho = 1.0;

float lightCoord[4] = {0.0, 0.0, 0.0, 0.0};

float ambientColor[4] = {1.0, 1.0, 1.0, 1.0};
float diffuseColor[4] = {0.5, 0.5, 0.5, 1.0};
float specularColor[4] = {1.0, 1.0, 1.0, 1.0};
float specularExponent = 5.0;

// Shapes
struct shape {
	int type;
	float angles[3];
	float moves[3];
	float sizes[3];

	float* vertices;
	GLushort *elements;
	float* normals;
	float* uvMap;
	int nVertices, nNormals, nElements, nUVs;

	bool bufferInit;
	GLuint buffer, ebuffer, texture;
	int dataSize, eDataSize, nDataSize, nUVSize;

	string texFile;

	float sun;

	shape(int _type):
		type(_type){
		bufferInit = false;
	}
};
vector<shape> shapes;


void 
createNewShape()
{
	for ALL_THE_DAMN_SHAPES {
		// clear your old shape
		clearShape();
    
		// create the new shape...should be a switch here
		switch (it->type)
		{
			case CUBE: makeCube (division1); break;
			case CYLINDER: makeCylinder (1.0, division1, division2); break;
			case CONE: makeCone (1.0, division1, division2); break;
			case SPHERE: makeSphere (1.0, division1); break;
			case BG: makeBG (); break;
		}
    
		// get the points for your shape
		it->nVertices = nVertices();
		it->nElements = nVertices() / 3;
		it->nNormals = nNormals();
		it->nUVs = nUVcoords();

		it->dataSize = it->nVertices * 4 * sizeof (float);
		it->nDataSize = it->nNormals * sizeof (float);
		it->eDataSize = it->nVertices * sizeof (GLushort);
		it->nUVSize = it->nUVs * sizeof (float);

		it->vertices = new float[it->dataSize];
		memcpy (it->vertices, getVertices(), it->dataSize);

		it->normals = new float[it->nDataSize];
		memcpy (it->normals, getNormals(), it->nDataSize);

		it->uvMap = new float[it->nUVSize];
		memcpy (it->uvMap, getUVmap(), it->nUVSize);

		it->elements = new GLushort[it->eDataSize];
		memcpy (it->elements, getElements(), it->eDataSize);

    
		// setup the vertex buffer
		if (it->bufferInit) glDeleteBuffers (1, &it->buffer);
		glGenBuffers( 1, &it->buffer );
		glBindBuffer( GL_ARRAY_BUFFER, it->buffer );
		glBufferData( GL_ARRAY_BUFFER, it->dataSize + it->nDataSize + it->nUVSize, 0, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, it->dataSize, it->vertices);
		glBufferSubData( GL_ARRAY_BUFFER, it->dataSize, it->nDataSize, it->normals);
		glBufferSubData( GL_ARRAY_BUFFER, it->dataSize + it->nDataSize, it->nUVSize, it->uvMap);
    
		// setup the element buffer
		if (it->bufferInit) glDeleteBuffers (1, &it->ebuffer);
		glGenBuffers( 1, &it->ebuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, it->ebuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, it->eDataSize, it->elements, GL_STATIC_DRAW );

		// setup the texture buffer
		if (it->bufferInit) glDeleteTextures (1, &it->texture);
		it->texture = SOIL_load_OGL_texture(
			it->texFile.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			0
			);
		if ( it->texture == 0 )
			printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
		glBindTexture(GL_TEXTURE_2D, it->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		printf("Info: Buffer: %i EBuffer: %i Texture: %i\n", it->buffer, it->ebuffer, it->texture);
		
		it->bufferInit = true;
	}
    
	glUseProgram( program );
    
    //  uniform variables
	// Object
    theta = glGetUniformLocation(program, "theta");
	trans = glGetUniformLocation(program, "trans");
	scale = glGetUniformLocation(program, "scale");

	// Camera
	cPosition = glGetUniformLocation(program, "cPosition");
	cLookAt = glGetUniformLocation(program, "cLookAt");
	cUp = glGetUniformLocation(program, "cUp");

	// Projection
	gLeft = glGetUniformLocation(program, "left");
	gRight = glGetUniformLocation(program, "right");
	gTop = glGetUniformLocation(program, "top");
	gBottom = glGetUniformLocation(program, "bottom");
	gNear = glGetUniformLocation(program, "near");
	gFar = glGetUniformLocation(program, "far");

	doOrtho = glGetUniformLocation(program, "doOrtho");

	// Light
	lightPosition = glGetUniformLocation(program, "LightPosition");
	ambient = glGetUniformLocation(program, "ambient");
	diffuse = glGetUniformLocation(program, "diffuse");
	specular = glGetUniformLocation(program, "specular");
	specexp = glGetUniformLocation(program, "specexp");

	// Misc
	outline = glGetUniformLocation(program, "outline");
	sun = glGetUniformLocation(program, "sun");

}


// OpenGL initialization 
void init() {
    
    // Load shaders and use the resulting shader program
    program = setUpAShader( "vshader.glsl", "fshader.glsl" );
    if (!program) {
        cerr << "Error setting up shaders" << endl;
        exit(1);
    }
    
    glEnable( GL_DEPTH_TEST ); 
	glEnable( GL_TEXTURE_2D );
	glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glClearColor( 0.0, 0.0, 0.0, 1.0 );

	// EARTH
	shape earth = shape(SPHERE);
	earth.angles[0] = 0.0;	earth.angles[1] = 0.0;	earth.angles[2] = 0.0;
	earth.moves[0]	 = 0.0;	earth.moves[1]  = 0.0; earth.moves[2]  = 0.0;
	earth.sizes[0]  = 0.2; earth.sizes[1]  = 0.2; earth.sizes[2]  = 0.2;
	earth.texFile = "kRsmm.png"; earth.sun = 0.0;



	// MOON
	shape moon = shape(SPHERE);
	moon.angles[0] = 0.0;	moon.angles[1] = 0.0;	moon.angles[2] = 0.0;
	moon.moves[0]	 = 1.0;	moon.moves[1]  = 0.0; moon.moves[2]  = 0.0;
	moon.sizes[0]  = 0.05; moon.sizes[1]  = 0.05; moon.sizes[2]  = 0.05;
	moon.texFile = "moon.jpg"; moon.sun = 0.0;



	// SUN
	shape sun = shape(SPHERE);
	sun.angles[0] = 0.0;	sun.angles[1] = 0.0;	sun.angles[2] = 0.0;
	sun.moves[0]	 = 0.0;	sun.moves[1]  = 0.0; sun.moves[2]  = 0.0;
	sun.sizes[0]  = 0.4; sun.sizes[1]  = 0.4; sun.sizes[2]  = 0.4;
	sun.texFile = "sun.jpg"; sun.sun = 1.0;



	// BACKGROUND
	shape background = shape(BG);
	background.angles[0] = 0.0;	background.angles[1] = 0.0;	background.angles[2] = 0.0;
	background.moves[0]	 = 0.0;	background.moves[1]  = 0.0; background.moves[2]  = 2.0;
	background.sizes[0]  = 2.0; background.sizes[1]  = 2.0; background.sizes[2]  = 1.0;
	background.texFile = "space.jpg"; background.sun = 1.0;

	
	shapes.push_back(earth);
	shapes.push_back(moon);
	shapes.push_back(sun);
	//shapes.push_back(background);

    
    // initally create shapes
    createNewShape();

}


void
display( void )
{
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    for ALL_THE_DAMN_SHAPES {

		//  Uniform values
		// Object Transforms
		glUniform3fv (theta, 1, it->angles);
		glUniform3fv (trans, 1, it->moves);
		glUniform3fv (scale, 1, it->sizes);

		// Camera
		glUniform3fv (cPosition, 1, cameraPosition);
		glUniform3fv (cLookAt, 1, lookAt);
		glUniform3fv (cUp, 1, up);

		// Projection
		glUniform1f (gLeft, fLeft);
		glUniform1f (gRight, fRight);
		glUniform1f (gTop, fTop);
		glUniform1f (gBottom, fBottom);
		glUniform1f (gNear, fNear);
		glUniform1f (gFar, fFar);
		glUniform1i (doOrtho, ortho);

		// Lighting
		glUniform4fv (lightPosition, 1, lightCoord);
		glUniform4fv (ambient, 1, ambientColor);
		glUniform4fv (diffuse, 1, diffuseColor);
		glUniform4fv (specular, 1, specularColor);
		glUniform1f  (specexp, specularExponent);

		// SUUUNNNNLIIIIGHHHHTTTT
		glUniform1f (sun, it->sun);


		GLuint vPosition = glGetAttribLocation( program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
							  BUFFER_OFFSET(0) );

		GLuint vNormal = glGetAttribLocation( program, "vNormal" );
		glEnableVertexAttribArray( vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
							  BUFFER_OFFSET(it->dataSize) );

		GLuint vTexCoords = glGetAttribLocation(program, "vTexCoord");
		glEnableVertexAttribArray(vTexCoords);
		glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0,
					BUFFER_OFFSET(it->dataSize + it->nDataSize) );
		glBindTexture(GL_TEXTURE_2D, it->texture);

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, it->ebuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, it->eDataSize, it->elements, GL_STATIC_DRAW );

		it->bufferInit = true;

		// clear and draw params
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT, GL_FILL);

		// draw your shape
		glUniform1f( outline, 0.0 );
		glDrawElements (GL_TRIANGLES, it->nVertices,  GL_UNSIGNED_SHORT, (void *)0);

		// Now for the outline
		glCullFace(GL_FRONT);
		glLineWidth(5);
		glPolygonMode(GL_BACK, GL_LINE);

		glUniform1f( outline, 1.0 );
		glDrawElements (GL_TRIANGLES, it->nVertices,  GL_UNSIGNED_SHORT, (void *)0);
	}
    
    // swap the buffers
    glutSwapBuffers();
}


void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {

        case 033:  // Escape key
        case 'q': case 'Q':
            exit( 0 );
        break;
    } 
    
    glutPostRedisplay();
}

void rotateShape (int i) {
	for ALL_THE_DAMN_SHAPES {
		it->angles[0] += 1.0;
		it->angles[1] += 1.0;

		if (it->angles[0] >= 360.0) it->angles[0] -= 360.0;
		if (it->angles[1] >= 360.0) it->angles[1] -= 360.0;
	}

	float eRadius = 1.60;
	float mRadius = 0.5;
	float cRadius = 1.5;

	shapes[0].moves[0] = sin(timePassed*3) * eRadius;
	shapes[0].moves[1] = cos(timePassed*3) * eRadius;

	shapes[1].moves[0] = (sin(timePassed * 24.0) * mRadius) + shapes[0].moves[0];
	shapes[1].moves[1] = (cos(timePassed * 24.0) * mRadius) + shapes[0].moves[1];
	timePassed += 0.005;

	if (timePassed > 5.0){
		for (int i = 0; i < 3; i++) shapes[2].sizes[i] += 0.005;
	}

	cameraPosition[0] = cos(timePassed / 2.0) * cRadius;
	cameraPosition[1] = sin(timePassed		) * cRadius;
	
	
	glutTimerFunc(50, rotateShape, 1);
	glutPostRedisplay();
}

#include <iostream>
using namespace std;

int main (int argc, char **argv)
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 1024, 1024);
    glutCreateWindow( "CG1 - Tessellation Assignment" );
    
#ifndef __APPLE__
    glewInit();
#endif
    
    init();
    
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	
	glutTimerFunc(50, rotateShape, 1);
    
    glutMainLoop();
    return 0;
}

