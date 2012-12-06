/**
 * shapes.cpp
 *
 * Routines for tessellating a number of basic shapes
 *
 * Students are to supply their implementations for the
 * functions in this file using the function "addTriangle()" to do the 
 * tessellation.
 *
 * Submitted by: Adrian Aiken (aca6943)
 */

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include "simpleShape.h"
#include "cg1Shape.h"
#include <math.h>
#include <vector>

#include <cstdio>

using namespace std;

// useful constant for cone circle points
#define DEGREE_RADIAN_FACTOR	.0174532925
#define RADIANS_IN_CIRCLE		6.28318530718
#define BOTTOM					-1.0
#define TOP						1.0
#define ICO_CONST				0.61803398875
#define T(a, b, c)				tri = new vector<Vector3>();\
								tri->push_back(vertexes[a]);\
								tri->push_back(vertexes[b]);\
								tri->push_back(vertexes[c]);\
								triangles.push_back(*tri);

/**
 * makeCube - Create a unit cube, centered at the origin, with a given number
 * of subdivisions in each direction on each face.
 *
 * @param subdivision - number of equal subdivisons to be made in each 
 *        direction along each face
 *
 * Can only use calls to addTriangle()
 */
void makeCube (int subdivisions)
{
	// Makes cube triangles easier to compute
	float step = (TOP + (-BOTTOM)) / (float)subdivisions;

	//  Each face will be subdivided into subdivisions^2 squares
	//  Each pair of triangles will be part of the following square:
	//		p3 *-----* p4
	//		   |   / |
	//		   | /   |
	//		p1 *-----* p2
	

	///// Front face (z = TOP) /////
	for (int x = 0; x < subdivisions; ++x)
	for (int y = 0; y < subdivisions; ++y)
	{
		// Define the square
		Point3 p1 = Point3(BOTTOM + (step*(x)  ), BOTTOM + (step*(y)  ), TOP);
		Point3 p2 = Point3(BOTTOM + (step*(x+1)), BOTTOM + (step*(y)  ), TOP);
		Point3 p3 = Point3(BOTTOM + (step*(x)  ), BOTTOM + (step*(y+1)), TOP);
		Point3 p4 = Point3(BOTTOM + (step*(x+1)), BOTTOM + (step*(y+1)), TOP);
		// Add the square's triangles
		addTriangle(p1, p2, p4);
		addTriangle(p1, p4, p3);
	}

	///// Back Face (z = BOTTOM) /////
	for (int x = 0; x < subdivisions; ++x)
	for (int y = 0; y < subdivisions; ++y)
	{
		// Define the square
		Point3 p1 = Point3(BOTTOM + (step*(x)  ), BOTTOM + (step*(y)  ), BOTTOM);
		Point3 p2 = Point3(BOTTOM + (step*(x+1)), BOTTOM + (step*(y)  ), BOTTOM);
		Point3 p3 = Point3(BOTTOM + (step*(x)  ), BOTTOM + (step*(y+1)), BOTTOM);
		Point3 p4 = Point3(BOTTOM + (step*(x+1)), BOTTOM + (step*(y+1)), BOTTOM);
		// Add the square's triangles
		addTriangle(p4, p2, p1);
		addTriangle(p3, p4, p1);
	}

	///// Bottom face (y = BOTTOM) /////
	for (int x = 0; x < subdivisions; ++x)
	for (int z = 0; z < subdivisions; ++z)
	{
		// Define the square
		Point3 p1 = Point3(BOTTOM + (step*(x)  ), BOTTOM, BOTTOM + (step*(z)  ));
		Point3 p2 = Point3(BOTTOM + (step*(x+1)), BOTTOM, BOTTOM + (step*(z)  ));
		Point3 p3 = Point3(BOTTOM + (step*(x)  ), BOTTOM, BOTTOM + (step*(z+1)));
		Point3 p4 = Point3(BOTTOM + (step*(x+1)), BOTTOM, BOTTOM + (step*(z+1)));
		// Add the square's triangles
		addTriangle(p1, p2, p4);
		addTriangle(p1, p4, p3);
	}

	///// Top Face (y = TOP) /////
	for (int x = 0; x < subdivisions; ++x)
	for (int z = 0; z < subdivisions; ++z)
	{
		// Define the square
		Point3 p1 = Point3(BOTTOM + (step*(x)  ), TOP, BOTTOM + (step*(z)  ));
		Point3 p2 = Point3(BOTTOM + (step*(x+1)), TOP, BOTTOM + (step*(z)  ));
		Point3 p3 = Point3(BOTTOM + (step*(x)  ), TOP, BOTTOM + (step*(z+1)));
		Point3 p4 = Point3(BOTTOM + (step*(x+1)), TOP, BOTTOM + (step*(z+1)));
		// Add the square's triangles
		addTriangle(p4, p2, p1);
		addTriangle(p3, p4, p1);
	}

	///// Right face (x = TOP) /////
	for (int y = 0; y < subdivisions; ++y)
	for (int z = 0; z < subdivisions; ++z)
	{
		// Define the square
		Point3 p1 = Point3(TOP, BOTTOM + (step*(y)  ), BOTTOM + (step*(z)  ));
		Point3 p2 = Point3(TOP, BOTTOM + (step*(y+1)), BOTTOM + (step*(z)  ));
		Point3 p3 = Point3(TOP, BOTTOM + (step*(y)  ), BOTTOM + (step*(z+1)));
		Point3 p4 = Point3(TOP, BOTTOM + (step*(y+1)), BOTTOM + (step*(z+1)));
		// Add the square's triangles
		addTriangle(p1, p2, p4);
		addTriangle(p1, p4, p3);
	}

	///// Left Face (x = BOTTOM) /////
	for (int y = 0; y < subdivisions; ++y)
	for (int z = 0; z < subdivisions; ++z)
	{
		// Define the square
		Point3 p1 = Point3(BOTTOM, BOTTOM + (step*(y)  ), BOTTOM + (step*(z)  ));
		Point3 p2 = Point3(BOTTOM, BOTTOM + (step*(y+1)), BOTTOM + (step*(z)  ));
		Point3 p3 = Point3(BOTTOM, BOTTOM + (step*(y)  ), BOTTOM + (step*(z+1)));
		Point3 p4 = Point3(BOTTOM, BOTTOM + (step*(y+1)), BOTTOM + (step*(z+1)));
		// Add the square's triangles
		addTriangle(p4, p2, p1);
		addTriangle(p3, p4, p1);
	}
	
}


/**
 * makeCylinder - Create polygons for a cylinder with unit height, centered at
 * the origin, with separate number of radial subdivisions and height 
 * subdivisions.
 *
 * @param radius - Radius of the base of the cylinder
 * @param radialDivision - number of subdivisions on the radial base
 * @param heightDivisions - number of subdivisions along the height
 *
 * Can only use calls to addTriangle()
 */
void makeCylinder (float radius, int radialDivisions, int heightDivisions)
{
	// minimum sides is 3
    if( radialDivisions < 3 ) {
        radialDivisions = 3;
    }
	
	// Angle & height steps to consider
	double angleStep = RADIANS_IN_CIRCLE / (double)radialDivisions;
	double heightStep = (TOP + (-BOTTOM)) / (double)heightDivisions;


	
	// Set of reference points to consider as we move down the cylinder
	vector<Point3> circumfrence;
	for (int i = 0; i < radialDivisions; ++i)
		circumfrence.push_back( Point3( cos( angleStep * i ) * radius,  // x
										0,							    // y
										sin( angleStep * i ) * radius));// z
	



	// For Top and Bottom Layers of the cylinder, we go clockwise around
	//  the top, going center, point i, point i+1 to produce the triangle

	// Create the top layer of the Cylinder
	Point3 top = Point3(0, TOP, 0);
	for (int i = 0; i < radialDivisions; ++i){
		Point3 a = Point3(circumfrence[i]);
		Point3 b = Point3(circumfrence[(i+1) % radialDivisions]);
		a.y = b.y = TOP;

		addTriangle(top, b, a);
	}
	// Now for the bottom layer
	Point3 bottom = Point3(0, BOTTOM, 0);
	for (int i = 0; i < radialDivisions; ++i){
		Point3 a = Point3(circumfrence[i]);
		Point3 b = Point3(circumfrence[(i+1) % radialDivisions]);
		a.y = b.y = BOTTOM;

		addTriangle(b, bottom, a);
	}


	// Finally the sides of the Cylinder. 
	//  Each pair of triangles will be part of the following square:
	//		p3 *-----* p4
	//		   |   / |
	//		   | /   |
	//		p1 *-----* p2
	for (int i = 0; i < heightDivisions; ++i)
	for (int j = 0; j < radialDivisions; ++j)
	{
		Point3 p1 = Point3(circumfrence[j]);
		Point3 p2 = Point3(circumfrence[(j+1) % radialDivisions]);
		
		p1.y = p2.y = BOTTOM + (heightStep * i);
		
		Point3 p3 = Point3(p1);
		Point3 p4 = Point3(p2);

		p3.y += heightStep;
		p4.y += heightStep;

		addTriangle(p4, p2, p1);
		addTriangle(p3, p4, p1);
	}

}


/**
 * makeCone - Create polygons for a cone with unit height, centered at the
 * origin, with separate number of radial subdivisions and height 
 * subdivisions.
 *
 * @param radius - Radius of the base of the cone
 * @param radialDivision - number of subdivisions on the radial base
 * @param heightDivisions - number of subdivisions along the height
 *
 * Can only use calls to addTriangle()
 */
void makeCone (float radius, int radialDivisions, int heightDivisions)
{
	// minimum sides is 3
    if( radialDivisions < 3 ) {
        radialDivisions = 3;
    }
	
	// Angle & height steps to consider
	double angleStep = RADIANS_IN_CIRCLE / (double)radialDivisions;
	double heightStep = (TOP + (-BOTTOM)) / (double)heightDivisions;
	double radStep = radius / (double)heightDivisions;

	
	// Set of reference points to consider as we move up the cone
	vector<Point3> circumfrence;
	for (int i = 0; i < radialDivisions; ++i)
		circumfrence.push_back( Point3( cos( angleStep * i ) * radius,  // x
										0,							    // y
										sin( angleStep * i ) * radius));// z

	// Set of deltas to be used as the cone is moved inwards
	vector<Point3> circDelta = vector<Point3>(circumfrence);
	for (int i = 0; i < radialDivisions; ++i)
	{
		circDelta[i].x *= radStep;
		circDelta[i].z *= radStep;
	}
	
	// Create the base of the Cone
	Point3 bottom = Point3(0, BOTTOM, 0);
	for (int i = 0; i < radialDivisions; ++i){
		Point3 a = Point3(circumfrence[i]);
		Point3 b = Point3(circumfrence[(i+1) % radialDivisions]);
		a.y = b.y = BOTTOM;

		addTriangle(b, bottom, a);
	}

	double curRad = 1.0;

	// Finally the sides of the Cone. 
	//  Each pair of triangles will be part of the following square:
	//		p3 *-----* p4
	//		   |   / |
	//		   | /   |
	//		p1 *-----* p2
	for (int i = 0; i < heightDivisions; ++i)
	{
		for (int j = 0; j < radialDivisions; ++j)
		{
			Point3 p1 = Point3(circumfrence[j]);
			Point3 p2 = Point3(circumfrence[(j+1) % radialDivisions]);
		
			p1.y = p2.y = BOTTOM + (heightStep * i);
		
			Point3 p3 = Point3(p1);
			Point3 p4 = Point3(p2);

			p3.y += heightStep;
			p4.y += heightStep;

			// Adjust the point's values for the slopes
			p1.x -= circDelta[j].x * i;
			p1.z -= circDelta[j].z * i;
			p2.x -= circDelta[(j+1) % radialDivisions].x * i;
			p2.z -= circDelta[(j+1) % radialDivisions].z * i;
			p3.x -= circDelta[j].x * (i+1);
			p3.z -= circDelta[j].z * (i+1);
			p4.x -= circDelta[(j+1) % radialDivisions].x * (i+1);
			p4.z -= circDelta[(j+1) % radialDivisions].z * (i+1);

			addTriangle(p4, p2, p1);
			addTriangle(p3, p4, p1);
		}

		curRad -= radStep;
	}

}


/**
 * makeSphere - Create sphere of a given radius, centered at the origin, 
 * using spherical coordinates with separate number of thetha and 
 * phi subdivisions.
 *
 * @param radius - Radius of the sphere
 * @param subdivisions - number of subdivisions
 *
 * Can only use calls to addTriangle, and the vertices vector.
 */

void recurseTriangle(vector<Vector3> triangle, int subdivisions, float radius);
void makeSphere (float radius, int subdivisions)
{
	// don't go above 5 subdivisions
	if (subdivisions > 5) {
		subdivisions = 5;
	}

	// Initialize icosahedron with the 12 points
	vector<Vector3> vertexes;
	vertexes.push_back(Vector3( 0, ICO_CONST, -1)); vertexes.push_back(Vector3(-ICO_CONST,  1, 0));
	vertexes.push_back(Vector3( ICO_CONST,  1, 0)); vertexes.push_back(Vector3(0,  ICO_CONST,  1));
	vertexes.push_back(Vector3(-1, 0,  ICO_CONST)); vertexes.push_back(Vector3(0, -ICO_CONST,  1));
	vertexes.push_back(Vector3( 1, 0,  ICO_CONST)); vertexes.push_back(Vector3( 1, 0, -ICO_CONST));
	vertexes.push_back(Vector3(0, -ICO_CONST, -1)); vertexes.push_back(Vector3(-1, 0, -ICO_CONST));
	vertexes.push_back(Vector3(-ICO_CONST, -1, 0)); vertexes.push_back(Vector3( ICO_CONST, -1, 0));

		
	// Creating a vector of all the triangles in the icosahedron
	//  using the above points
	vector< vector<Vector3> > triangles;
	vector<Vector3>* tri;

	T( 0, 1, 2); T( 3, 2, 1); T( 3, 4, 5); T( 3, 5, 6);
	T( 0, 7, 8); T( 0, 8, 9); T( 5,10,11); T( 8,11,10);
	T( 1, 9, 4); T(10, 4, 9); T( 2, 6, 7); T(11, 7, 6);
	T( 3, 1, 4); T( 3, 6, 2); T( 0, 9, 1); T( 0, 2, 7);
	T( 8,10, 9); T( 8, 7,11); T( 5, 4,10); T( 5,11, 6);

	/// Adding UVmap info to each triangle
	// number of points horizontally and vertically
	float w = 5.5f, h = 3.f;
	vector<float> uMap;
	vector<float> vMap;

	// create a set of points to refrence
	for (float f = 0.5; f <= 5.0; f += 1) {
		uMap.push_back(f / w);
		vMap.push_back(0);
	}
	for (float f = 0.0; f <= 5.0; f += 1) {
		uMap.push_back(f / w);
		vMap.push_back(1 / h);
	}
	for (float f = 0.5; f <= 5.5; f += 1) {
		uMap.push_back(f / w);
		vMap.push_back(2 / h);
	}
	for (float f = 1.0; f <= 5.0; f += 1) {
		uMap.push_back(f / w);
		vMap.push_back( 1.0 );
	}

#define UV(t, a, b, c)	triangles[t-1][0].u = uMap[a]; triangles[t-1][0].v = vMap[a];\
						triangles[t-1][1].u = uMap[b]; triangles[t-1][1].v = vMap[b];\
						triangles[t-1][2].u = uMap[c]; triangles[t-1][2].v = vMap[c];
	// Set the UV points for the triangle's points
	UV(1, 0, 5, 6);		UV(2, 11, 6, 5);	UV(3, 16, 15, 21);	UV(4, 11, 17, 12);
	UV(5, 2, 7, 8);		UV(6, 3, 8, 9);		UV(7, 19, 14, 13);	UV(8, 8, 13, 14);
	UV(9, 10, 9, 15);	UV(10, 14, 15, 9);	UV(11, 6, 12, 7);	UV(12, 13, 7, 12);
	UV(13, 16, 10, 15);	UV(14, 11, 12, 6);	UV(15, 4, 9, 10);	UV(16, 1, 6, 7);
	UV(17, 8, 14, 9);	UV(18, 8, 7, 13);	UV(19, 20, 15, 14);	UV(20, 18, 13, 12);

	for each(vector<Vector3> triangle in triangles)
		recurseTriangle(triangle, subdivisions, radius);

}

void recurseTriangle(vector<Vector3> triangle, int subdivisions, float radius){

	// Just 1 subdivision is a triangle ready to be put onto the screen
	if (subdivisions == 1){
		vector<Point3> pTriangle;
		for each(Vector3 v in triangle) {
			v.normalize();
			v *= radius;
			pTriangle.push_back(Point3(v.x, v.y, v.z, v.u, v.v));
		}
		addTriangle(pTriangle[0], pTriangle[1], pTriangle[2], true);

		return;
	}

	// Still more subdivisions to go, which is computed recursively.
	// Split the triangle into four portions (as seen below) and the points
	//  are sent into the next recursive call, one subdivision less
	//				  2
	//                /\
	//             c /__\ b
	//              /\  /\
	//            0/__\/__\1
	//                a

	Vector3 a = (triangle[0] + triangle[1]) * 0.5f;
	a.u = (triangle[0].u + triangle[1].u)   * 0.5f;
	a.v = (triangle[0].v + triangle[1].v)	* 0.5f;
	Vector3 b = (triangle[1] + triangle[2]) * 0.5f;
	b.u = (triangle[1].u + triangle[2].u)	* 0.5f;
	b.v = (triangle[1].v + triangle[2].v)	* 0.5f;
	Vector3 c = (triangle[2] + triangle[0]) * 0.5f;
	c.u = (triangle[2].u + triangle[0].u)	* 0.5f;
	c.v = (triangle[2].v + triangle[0].v)	* 0.5f;

	vector<Vector3> ll;
	ll.push_back(triangle[0]); ll.push_back(a); ll.push_back(c);
	recurseTriangle(ll, subdivisions-1, radius);
	vector<Vector3> lr;
	lr.push_back(a); lr.push_back(triangle[1]); lr.push_back(b);
	recurseTriangle(lr, subdivisions-1, radius);
	vector<Vector3> up;
	up.push_back(c); up.push_back(b); up.push_back(triangle[2]);
	recurseTriangle(up, subdivisions-1, radius);
	vector<Vector3> mi;
	mi.push_back(a); mi.push_back(b); mi.push_back(c);
	recurseTriangle(mi, subdivisions-1, radius);

};


/**
 * makeBG - Creates a simple background to be used
 *
 * @param top, bottom, left, right, far - view frustrum parameters
 */

 void makeBG ()
 {
	//  Each pair of triangles will be part of the following square
	//		p3 *-----* p4
	//		   |   / |
	//		   | /   |
	//		p1 *-----* p2

	Point3 p1 = Point3(BOTTOM, BOTTOM, 0.0, 0.0, 0.0);
	Point3 p2 = Point3(TOP, BOTTOM, 0.0, 1.0, 0.0);
	Point3 p3 = Point3(BOTTOM, TOP, 0.0, 0.0, 1.0);
	Point3 p4 = Point3(TOP, TOP, 0.0, 1.0, 1.0);

	addTriangle(p1, p2, p4, true);
	addTriangle(p1, p4, p3, true);
 }