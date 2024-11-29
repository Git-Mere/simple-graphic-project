/*!************************************************************************
\file HalfSpace.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Clipping Programming Assignment
\date 11-29-2023
\brief
Implement function about halfspace
**************************************************************************/

#include "HalfSpace.h"

/*!*****************************************************************************
\brief
	constructor take normal vector and point c on boundary of halfspace

\param m
	normal vector
\param C
	Point

*******************************************************************************/
HalfSpace::HalfSpace(const Vector& m, const Point& C)
{
	x = m.x;
	y = m.y;
	z = m.z;

	w = -(m.x * C.x + m.y * C.y + m.z * C.z);
}

/*!*****************************************************************************
\brief
	constructor take points to make normal vector and interior point P

\param A
	point 1
\param B
	point 2
\param C
	point 3
\param P
	interior Point

*******************************************************************************/
HalfSpace::HalfSpace(const Point& A, const Point& B, const Point& C, const Point& P)
{
	Vector normal = cross(Vector(B - A), Vector(C - A));

	x = normal.x;
	y = normal.y;
	z = normal.z;

	float tmp_w = -(normal.x * A.x + normal.y * A.y + normal.z * A.z);

	HalfSpace tmp(x, y, z, tmp_w);
	if (dot(tmp, P) <= 0) {
		w = tmp_w;
	}
	else {
		x = -x;
		y = -y;
		z = -z;
		w = -tmp_w;
	}
}

/*!*****************************************************************************
\brief
	constructor take cube mesh half spaces

\param cube
	if true, this take cube half spaces
	otherwise default poly

*******************************************************************************/
Polyhedron::Polyhedron(bool cube)
{
	if (cube) {
		half_spaces.push_back(HalfSpace(0, 0, -1, -1)); // near
		half_spaces.push_back(HalfSpace(0, 0, 1, -1)); // far
		half_spaces.push_back(HalfSpace(-1, 0, 0, -1)); // left
		half_spaces.push_back(HalfSpace(1, 0, 0, -1)); // right
		half_spaces.push_back(HalfSpace(0, -1, 0, -1)); // bottom
		half_spaces.push_back(HalfSpace(0, 1, 0, -1)); // top
	}
	
}

/*!*****************************************************************************
\brief
	constructor take camera frustum half spaces

\param fov
	ratio
\param a
	aspect
\param N
	near
\param F
	far

*******************************************************************************/
Polyhedron::Polyhedron(float fov, float a, float N, float F)
{
	float D = (N + F) / 2;
	float width = 2 * std::tan(fov / 2) * D;
	//float height = width / a;

	float near_width = (N * width / D);
	float near_height = a * near_width;

	float far_width = (F * width / D);
	float far_height = a * far_width;

	Point vertice1 = Point(near_width / 2, near_height / 2, -N);
	//Point vertice2 = Point(near_width / 2, -near_height / 2, -N);
	Point vertice3 = Point(-near_width / 2, -near_height / 2, -N);
	Point vertice4 = Point(-near_width / 2, near_height / 2, -N);

	Point vertice5 = Point(far_width / 2, far_height / 2, -F);
	Point vertice6 = Point(far_width / 2, -far_height / 2, -F);
	//Point vertice7 = Point(-far_width / 2, -far_height / 2, -F);
	Point vertice8 = Point(-far_width / 2, far_height / 2, -F);

	//near
	Vector normal1 = cross(Vector(vertice4 - vertice1), Vector(vertice3 - vertice1));
	float tmp_w = -(normal1.x * vertice1.x + normal1.y * vertice1.y + normal1.z * vertice1.z);
	HalfSpace half1(normal1.x, normal1.y, normal1.z, tmp_w);


	//far
	Vector normal2 = cross(Vector(vertice6 - vertice5), Vector(vertice8 - vertice5));
	tmp_w = -(normal2.x * vertice5.x + normal2.y * vertice5.y + normal2.z * vertice5.z);
	HalfSpace half2(normal2.x, normal2.y, normal2.z, tmp_w);

	half_spaces.push_back(half1);
	half_spaces.push_back(half2);
}


/*!*****************************************************************************
\brief
	check this point is inside of poly

\param polyhedron
	poly object
\param P
	point

*******************************************************************************/
bool contains(const Polyhedron& polyhedron, const Hcoord& P)
{
	if (P.w < 0) {
		return false;
	}
	for (const HalfSpace& tmp : polyhedron.half_spaces) {
		if (dot(tmp, P) > 0) {
			return false;
		}
	}
	
	return true;
}
