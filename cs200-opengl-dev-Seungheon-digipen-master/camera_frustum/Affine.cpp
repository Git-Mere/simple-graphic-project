/*!************************************************************************
\file Affine.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Affine and Mesh Programming Assignment
\date 10-20-2023
\brief
Implement function about Affine
**************************************************************************/

#include "Affine.h"

Hcoord::Hcoord(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W)
{
}

Point::Point(float X, float Y, float Z) : Hcoord(X, Y, Z, 1)
{
}

Vector::Vector(float X, float Y, float Z) : Hcoord(X, Y, Z, 0)
{
}

Affine::Affine(void)
{
	row[0] = Hcoord(1, 0, 0, 0);
	row[1] = Hcoord(0, 1, 0, 0);
	row[2] = Hcoord(0, 0, 1, 0);
	row[3] = Hcoord(0, 0, 0, 1);
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
	row[0] = Hcoord(Lx.x, Ly.x, Lz.x, D.x);
	row[1] = Hcoord(Lx.y, Ly.y, Lz.y, D.y);
	row[2] = Hcoord(Lx.z, Ly.z, Lz.z, D.z);
	row[3] = Hcoord(0, 0, 0, 1);
}

Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
	Hcoord result;
	result.x = u.x + v.x;
	result.y = u.y + v.y;
	result.z = u.z + v.z;
	result.w = u.w + v.w;
	return result;
}

Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
	Hcoord result;
	result.x = u.x - v.x;
	result.y = u.y - v.y;
	result.z = u.z - v.z;
	result.w = u.w - v.w;
	return result;
}

Hcoord operator-(const Hcoord& v)
{
	return Hcoord(-v.x, -v.y, -v.z, -v.w);
}

Hcoord operator*(float r, const Hcoord& v)
{
	Hcoord result;
	result.x = r * v.x;
	result.y = r * v.y;
	result.z = r * v.z;
	result.w = r * v.w;
	return result;
}

/*!*****************************************************************************
\brief
	Compute multiply Matrix and Hcoord
\param A
	Matrix
\param v
	Hcoord
\return
  result Hcoord
*******************************************************************************/
Hcoord operator*(const Matrix& A, const Hcoord& v)
{
	Hcoord result;
	for (int i = 0; i < 4; i++) {
		result[i] = A[i][0] * v.x + A[i][1] * v.y + A[i][2] * v.z + A[i][3] * v.w;
	}
	return result;
}

/*!*****************************************************************************
\brief
	Compute multiply Matrix and Matrix
\param A
	Matrix 1
\param B
	Matrix 2
\return
  result Matrix
*******************************************************************************/
Matrix operator*(const Matrix& A, const Matrix& B)
{
	Matrix result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j] + A[i][3] * B[3][j];
		}
	}

	return result;
}

Matrix operator+(const Matrix& A, const Matrix& B)
{
	Matrix result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = A[i][j] + B[i][j];
		}
	}
	result[3][3] = 1;
	return result;
}

/*!*****************************************************************************
\brief
	Compute dot product Vector and Vector
\param u
	Vector 1
\param v
	Vector 2
\return
  result float value
*******************************************************************************/
float dot(const Vector& u, const Vector& v) 
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

/*!*****************************************************************************
\brief
	Compute Vector's distance
\param v
	target vector
\return
  Vector's distance
*******************************************************************************/
float abs(const Vector& v)
{
	return std::sqrt(dot(v, v));
}

/*!*****************************************************************************
\brief
	Make normal vector
\param v
	target vector
\return
  v's normal vector
*******************************************************************************/
Vector normalize(const Vector& v)
{
	float length = abs(v);
	if (length != 0) {
		float invLength = 1.0f / length;
		return Vector(v.x * invLength, v.y * invLength, v.z * invLength);
	}

		return v;

}

/*!*****************************************************************************
\brief
	To compute cross product
\param u
	vector 1
\param v
	vector 1
\return
  result of cross product
*******************************************************************************/
Vector cross(const Vector& u, const Vector& v)
{
	return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

/*!*****************************************************************************
\brief
	Make rotate matrix
\param t
	radian value
\param v
	axis vector
\return
  rotate matrix
*******************************************************************************/
Affine rotate(float t, const Vector& v)
{
	float c = std::cos(t);
	float s = std::sin(t);
	float oneMinusC = 1.0f - c;

	Vector axis = normalize(v);

	Affine result;

	Affine first = scale(c);
	Affine second = { Vector(axis.x * axis.x, axis.y * axis.x, axis.z * axis.x), Vector(axis.x * axis.y, axis.y * axis.y, axis.z * axis.y), Vector(axis.x * axis.z, axis.y * axis.z, axis.z * axis.z), Point() };
	Affine third = { Vector(0, axis.z, -axis.y), Vector(-axis.z, 0, axis.x), Vector(axis.y, -axis.x, 0), Point() };
	for (int i = 0; i < 4; i++) {
		second[i] = oneMinusC * second[i];
		third[i] = s * third[i];
	}
	result = first + second + third;

	return result;
}

Affine translate(const Vector& v)
{
	Affine result;
	result[0][3] = v.x;
	result[1][3] = v.y;
	result[2][3] = v.z;
	return result;
}

Affine scale(float r)
{
	Affine result;
	result[0][0] = r;
	result[1][1] = r;
	result[2][2] = r;
	return result;
}

Affine scale(float rx, float ry, float rz)
{
	Affine result;
	result[0][0] = rx;
	result[1][1] = ry;
	result[2][2] = rz;
	return result;
}

/*!*****************************************************************************
\brief
	Make inverse matrix
\param A
	target matrix
\return
  target matrix's inverse
*******************************************************************************/
Affine inverse(const Affine& A)
{
	float det = (A[0][0] * A[1][1] * A[2][2]) + (A[0][1] * A[1][2] * A[2][0]) + (A[0][2] * A[1][0] * A[2][1])
		- (A[0][2] * A[1][1] * A[2][0]) - (A[0][0] * A[1][2] * A[2][1]) - (A[0][1] * A[1][0] * A[2][2]);

	Vector first((A[1][1] * A[2][2]) - (A[1][2] * A[2][1]), -((A[1][0] * A[2][2]) - (A[1][2] * A[2][0])), (A[1][0] * A[2][1]) - (A[1][1] * A[2][0]));
	Vector second(-((A[0][1] * A[2][2]) - (A[0][2] * A[2][1])), ((A[0][0] * A[2][2]) - (A[0][2] * A[2][0])), -((A[0][0] * A[2][1]) - (A[0][1] * A[2][0])));
	Vector third((A[0][1] * A[1][2]) - (A[0][2] * A[1][1]), -((A[0][0] * A[1][2]) - (A[0][2] * A[1][0])), (A[0][0] * A[1][1]) - (A[0][1] * A[1][0]));
	Point fourth(-A[0][3], -A[1][3], -A[2][3]);

	Vector result_first = (1 / det) * first;
	Vector result_second = (1 / det) * second;
	Vector result_third = (1 / det) * third;


	Affine time_con(result_first, result_second, result_third, Point());
	Affine time_con2(Vector(1,0,0), Vector(0,1,0), Vector(0,0,1), fourth);

	Affine result = time_con * time_con2;

	return result;
}
