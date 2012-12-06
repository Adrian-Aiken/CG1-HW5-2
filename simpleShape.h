/**
 * header file for functions that manage the current shape 
 * being defined (including the addTriangles() function.
 *
 * Students are not to modify this file
 */

#ifndef __addTriangles__
#define __addTriangles__

#include "vecmath.h"
#include <vector>

extern std::vector<Point3> vertices;

void clearShape ();

void addTriangle (const Point3& p1, const Point3& p2, const Point3& p3, bool hasTexture = false);

GLushort *getElements ();

float *getVertices ();

int nVertices ();

float *getNormals ();

int nNormals ();

float *getUVmap ();

int nUVcoords ();

#endif
