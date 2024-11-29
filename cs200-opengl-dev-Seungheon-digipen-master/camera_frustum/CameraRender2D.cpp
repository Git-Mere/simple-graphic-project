/*!************************************************************************
\file CameraRender2D.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Transform Projections Programming Assignment
\date 11-17-2023
\brief
Implement function about CameraRender2D
**************************************************************************/

#include "CameraRender2D.h"
#include "Projection.h"

/*!*****************************************************************************
\brief
	Set Camera with default camera
\param r
	render object
*******************************************************************************/
CameraRender2D::CameraRender2D(Render& r) : render(r)
{
	Camera origin;
	SetCamera(origin);
}

CameraRender2D::~CameraRender2D(void)
{
	render.~Render();
}

void CameraRender2D::SetCamera(const Camera& cam)
{
	world2camera = WorldToCamera(cam);
	camera2ndc = CameraToNDC(cam);
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
void CameraRender2D::DisplayEdges(Mesh& m, const Affine& A, const Vector& color)
{
	cam_vertices.clear();
	ndc_vertices.clear();

	for (int i = 0; i < m.VertexCount(); ++i)
	{
		Hcoord world_vert = A * m.GetVertex(i);
		Hcoord cam_vert = world2camera * world_vert;
		cam_vertices.push_back(cam_vert);
	}

	for (int i = 0; i < m.VertexCount(); ++i)
	{
		Hcoord perspective = camera2ndc * cam_vertices[i];
		Point ndc_vert;
		ndc_vert.x = perspective.x / perspective.w;
		ndc_vert.y = perspective.y / perspective.w;
		ndc_vert.z = perspective.z / perspective.w;
		ndc_vert.w = perspective.w / perspective.w;
		ndc_vertices.push_back(ndc_vert);
	}

	for (int i = 0; i < m.EdgeCount(); ++i) {
		const Mesh::Edge& edge = m.GetEdge(i);
		if (cam_vertices[edge.index1].z < 0 && cam_vertices[edge.index2].z < 0) {
			render.SetColor(color);
			render.DrawLine(ndc_vertices[edge.index1], ndc_vertices[edge.index2]);
		}
	}
}


void CameraRender2D::DisplayEdges(std::vector<Hcoord> m, const Affine& A, const Vector& color)
{
	cam_vertices.clear();
	ndc_vertices.clear();
	size_t count = m.size();
	render.SetColor(color);

	for (int i = 0; i < count; ++i)
	{
		Hcoord world_vert = A * m[i];
		Hcoord cam_vert = world2camera * world_vert;
		cam_vertices.push_back(cam_vert);
	}

	for (int i = 0; i < count; ++i)
	{
		Hcoord perspective = camera2ndc * cam_vertices[i];
		Point ndc_vert;
		ndc_vert.x = perspective.x / perspective.w;
		ndc_vert.y = perspective.y / perspective.w;
		ndc_vert.z = perspective.z / perspective.w;
		ndc_vert.w = perspective.w / perspective.w;
		ndc_vertices.push_back(ndc_vert);
	}

	for (int i = 0; i < count - 1; ++i) {
		//const Mesh::Edge& edge = m.GetEdge(i);
		if (cam_vertices[i].z < 0 && cam_vertices[i + 1].z < 0) {
			render.DrawLine(ndc_vertices[i], ndc_vertices[i + 1]);
		}
	}
	if (cam_vertices[count - 1].z < 0 && cam_vertices[0].z < 0) {
		render.DrawLine(ndc_vertices[count - 1], ndc_vertices[0]);
	}
	//
}


/*!*****************************************************************************
\brief
	Display Face using render's FillTriangle function using back culling
\param m
	mesh data
\param A
	Model matrix
\param color
	set color
*******************************************************************************/
void CameraRender2D::DisplayFaces(Mesh& m, const Affine& A, const Vector& color)
{
	int count = m.VertexCount();
	std::vector<Point> world_vertices{};
	for (int i = 0; i < count; i++) {
		Point transformed_vertex = (A * m.GetVertex(i));
		world_vertices.push_back(transformed_vertex);
	}

	cam_vertices.clear();
	for (int i = 0; i < count; i++) {
		Point transformed_vertex = world2camera * (A * m.GetVertex(i));
		cam_vertices.push_back(transformed_vertex);
	}

	ndc_vertices.clear();
	for (int i = 0; i < count; i++) {
		Hcoord tmp = camera2ndc * cam_vertices[i];
		Point projected_vertex = 1 / tmp.w * tmp;
		ndc_vertices.push_back(projected_vertex);
	}
	Affine stad = inverse(world2camera);
	Vector L(stad[0][2], stad[1][2], stad[2][2]);
	Point E(stad[0][3], stad[1][3], stad[2][3]);

	for (int i = 0; i < m.FaceCount(); i++) {
		if (!(cam_vertices[m.GetFace(i).index1].z >= 0 || cam_vertices[m.GetFace(i).index2].z >= 0 || cam_vertices[m.GetFace(i).index3].z >= 0)) {
			Vector n = cross((world_vertices[m.GetFace(i).index3] - world_vertices[m.GetFace(i).index1]), (world_vertices[m.GetFace(i).index2] - world_vertices[m.GetFace(i).index1]));
			Vector PE = world_vertices[m.GetFace(i).index1] - E;
			float dotProduct = dot(PE, n);
			if (dotProduct > 0) {
				float child = std::abs(dot(L, n));
				float mom = abs(L) * abs(n);
				float res = child / mom;
				render.SetColor(res * color);

				render.FillTriangle(ndc_vertices[m.GetFace(i).index1], ndc_vertices[m.GetFace(i).index2], ndc_vertices[m.GetFace(i).index3]);
			}
		}
	}
}
