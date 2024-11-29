/*!************************************************************************
\file SimpleRender.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Camera Frustum Programming Assignment
\date 11-10-2023
\brief
Implement function about SimpleRender
**************************************************************************/

#include "SimpleRender.h"

/*!*****************************************************************************
\brief
	Set PersProj Matrix width E = (0,0,5), z = -1/2
\param r
	render object
*******************************************************************************/
SimpleRender::SimpleRender(Render& r) : render(r)
{
	PersProj[0][0] = 1;
	PersProj[1][1] = 1;
	PersProj[2][2] = 1 / 11.f;
	PersProj[2][3] = -5 / 11.f;
	PersProj[3][2] = -2 / 11.f;
	PersProj[3][3] = 10 / 11.f;
}

SimpleRender::~SimpleRender(void)
{
	delete &render;
}

/*!*****************************************************************************
\brief
	Display Edge using render's DrawLine function
\param m
	mesh data
\param A
	Model matrix
\param color
	set color
*******************************************************************************/
void SimpleRender::DisplayEdges(Mesh& m, const Affine& A,const Vector& color)
{
	int count = m.VertexCount();
	render.SetColor(color);

	proj_vertices.clear();
	for (int i = 0; i < count; i++) {
		Hcoord tmp = PersProj * A * m.GetVertex(i);
		Point projected_vertex = 1/tmp.w * tmp;
		proj_vertices.push_back(projected_vertex);
	}

	for (int i = 0; i < m.EdgeCount(); i++) {
		render.DrawLine(proj_vertices[m.GetEdge(i).index1], proj_vertices[m.GetEdge(i).index2]);
	}
}

/*!*****************************************************************************
\brief
	Display Edge using render's FillTriangle function, light vector = (0,0,1), using back culling
\param m
	mesh data
\param A
	Model matrix
\param color
	set color
*******************************************************************************/
void SimpleRender::DisplayFaces(Mesh& m, const Affine& A, const Vector& color)
{
	int count = m.VertexCount();

	world_vertices.clear();
	for (int i = 0; i < count; i++) {
		Point transformed_vertex = A * m.GetVertex(i);
		world_vertices.push_back(transformed_vertex);
	}

	proj_vertices.clear();
	for (int i = 0; i < count; i++) {
		Hcoord tmp = PersProj * world_vertices[i];
		Point projected_vertex = 1 / tmp.w * tmp;
		proj_vertices.push_back(projected_vertex);
	}

	Vector light(0, 0, 1);

	for (int i = 0; i < m.FaceCount(); i++) {
		Vector stad = cross((world_vertices[m.GetFace(i).index3] - world_vertices[m.GetFace(i).index1]), (world_vertices[m.GetFace(i).index2] - world_vertices[m.GetFace(i).index1]));
		
		float dotProduct = dot(stad, (proj_vertices[m.GetFace(i).index1] - Point(0, 0, 5)));
		if (dotProduct > 0) {
			float child = std::abs(dot(stad, light));
			float mom = abs(stad) * abs(light);
			float res = std::abs(child) / mom;
			render.SetColor(res * color);

			render.FillTriangle(proj_vertices[m.GetFace(i).index1], proj_vertices[m.GetFace(i).index2], proj_vertices[m.GetFace(i).index3]);
		}
	}
}
