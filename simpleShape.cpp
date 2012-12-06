/**
 *
 * simpleShape.cpp
 *
 * routines for adding triangles to create a new mesh
 *
 * Students are not to modify this file.
 *
 */

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <vector>
#include <iostream>
#include "simpleShape.h"

using namespace std;

/**
 * common variables...should probably make this a class and make these
 * data members
 */
vector<Point3> vertices;
vector<float>  normals;
vector<float>  uvMap;
float *pointArray = 0;
GLushort *elemArray = 0;
float *normalArray = 0;
float *uvArray = 0;


/**
 * clear the current shape
 */
void clearShape ()
{
    if (pointArray) {
        delete[] pointArray;
        delete[] elemArray;
		delete[] normalArray;
		delete[] uvArray;
        pointArray = 0;
        elemArray = 0;
		normalArray = 0;
		uvArray = 0;
	}
    vertices.clear();
	normals.clear();
	uvMap.clear();
}


/**
 * adds a triangle to the current shape
 */
void addTriangle (const Point3& p1, const Point3& p2, const Point3& p3, bool hasTexture)
{
    vertices.push_back (p1);
    vertices.push_back (p2);
    vertices.push_back (p3);

	// Add the normal information for the given triangle
	Vector3 U = p2 - p1;
	Vector3 V = p3 - p1;

	
	normals.push_back ((U.y * V.z) - (U.z * V.y));
	normals.push_back ((U.z * V.x) - (U.x * V.z));
	normals.push_back ((U.x * V.y) - (U.y * V.x));

	normals.push_back ((U.y * V.z) - (U.z * V.y));
	normals.push_back ((U.z * V.x) - (U.x * V.z));
	normals.push_back ((U.x * V.y) - (U.y * V.x));
	
	normals.push_back ((U.y * V.z) - (U.z * V.y));
	normals.push_back ((U.z * V.x) - (U.x * V.z));
	normals.push_back ((U.x * V.y) - (U.y * V.x));

	if (hasTexture) {
		uvMap.push_back(p1.u); uvMap.push_back(p1.v);
		uvMap.push_back(p2.u); uvMap.push_back(p2.v);
		uvMap.push_back(p3.u); uvMap.push_back(p3.v);
	}
}


/**
 * gets the vertex points for the current shape
 */
float *getVertices ()
{
    // delete the old point array of we have one
    if (pointArray) {
        delete[] pointArray;
    }
    
    // create and fill a new point array
    pointArray = new float[vertices.size()*4];
    for (int i=0; i < vertices.size(); i++) {
        pointArray[i*4] = vertices[i].x;
        pointArray[i*4+1] = vertices[i].y;
        pointArray[i*4+2] = vertices[i].z;
        pointArray[i*4+3] = 1.0;
    }
        
    return pointArray;
}

/**
 * gets the  array of elements for the  current shape
 */
GLushort *getElements ()
{
    // delete the old point array of we have one
    if (elemArray) {
        delete[] elemArray;
    }
    
    // create and fill a new point array
    elemArray = new GLushort[vertices.size()*4];
    for (int i=0; i < vertices.size(); i++) {
        elemArray[i] = i;
    }
    
    return elemArray;
}



/**
 * returns number of verticies in current shape
 */
int nVertices ()
{
    return vertices.size();
}


/**
 * gets the array of normals for the current shape
 */
float *getNormals ()
{
	// delete the old normal array if we have one
	if (normalArray) {
		delete[] normalArray;
	}

	// create and fill a new normal array
	normalArray = new float[normals.size()];
	for (int i = 0; i < normals.size(); i++) {
		normalArray[i] = normals[i];
	}

	return normalArray;
}

/**
 * returns the number of normals in the shape
 */
int nNormals ()
{
	return normals.size();
}

/**
 * gets an array of the coordinates for the UV map
 */
float *getUVmap () 
{
	// delete the old UV map if we have one
	if (uvArray) {
		delete[] uvArray;
	}

	// create and fill a new UV array
	uvArray = new float[uvMap.size()];
	for (int i = 0; i < uvMap.size(); i++){
		uvArray[i] = uvMap[i];
	}

	return uvArray;
}

/**
 * returns the number of UV coordinates stored
 */
int nUVcoords ()
{
	return uvMap.size();
}