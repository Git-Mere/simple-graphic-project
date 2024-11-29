/*!************************************************************************
\file CubeMesh.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Affine and Mesh Programming Assignment
\date 10-20-2023
\brief
Implement function about CubeMesh
**************************************************************************/

#include "CubeMesh.h"
const Point CubeMesh::vertices[8] = {
    Point(-1.0f, -1.0f, -1.0f),
    Point(1.0f, -1.0f, -1.0f),
    Point(1.0f, 1.0f, -1.0f),
    Point(-1.0f, 1.0f, -1.0f),
    Point(-1.0f, -1.0f, 1.0f),
    Point(1.0f, -1.0f, 1.0f),
    Point(1.0f, 1.0f, 1.0f),
    Point(-1.0f, 1.0f, 1.0f)
};

const Mesh::Face CubeMesh::faces[12] = {
    Mesh::Face(0, 2, 1),
    Mesh::Face(0, 3, 2),
    Mesh::Face(4, 5, 6),
    Mesh::Face(4, 6, 7),
    Mesh::Face(0, 1, 5),//
    Mesh::Face(0, 5, 4),//
    Mesh::Face(1, 2, 6),//
    Mesh::Face(1, 6, 5),//
    Mesh::Face(2, 3, 7),//
    Mesh::Face(2, 7, 6),//
    Mesh::Face(3, 0, 4),//
    Mesh::Face(3, 4, 7)//
};

const Mesh::Edge CubeMesh::edges[12] = {
    Mesh::Edge(0, 1),
    Mesh::Edge(1, 2),
    Mesh::Edge(2, 3),
    Mesh::Edge(3, 0),
    Mesh::Edge(4, 5),
    Mesh::Edge(5, 6),
    Mesh::Edge(6, 7),
    Mesh::Edge(7, 4),
    Mesh::Edge(0, 4),
    Mesh::Edge(1, 5),
    Mesh::Edge(2, 6),
    Mesh::Edge(3, 7)
};

int CubeMesh::VertexCount(void)
{
	return 8;
}

Point CubeMesh::GetVertex(int i)
{
    return vertices[i];
}

/*!*****************************************************************************
\brief
    Compute this cube's dimensions

\return
  cube's dimensions
*******************************************************************************/
Vector CubeMesh::Dimensions(void)
{
    Vector minVertex(vertices[0].x, vertices[0].y, vertices[0].z);
    Vector maxVertex(vertices[0].x, vertices[0].y, vertices[0].z);

    for (int i = 1; i < 8; i++) {
        minVertex.x = std::min(minVertex.x, vertices[i].x);
        minVertex.y = std::min(minVertex.y, vertices[i].y);
        minVertex.z = std::min(minVertex.z, vertices[i].z);

        maxVertex.x = std::max(maxVertex.x, vertices[i].x);
        maxVertex.y = std::max(maxVertex.y, vertices[i].y);
        maxVertex.z = std::max(maxVertex.z, vertices[i].z);
    }

    return maxVertex - minVertex;
}

/*!*****************************************************************************
\brief
    Compute this cube's center point

\return
  cube's center point
*******************************************************************************/
Point CubeMesh::Center(void)
{
    Vector minVertex(vertices[0].x, vertices[0].y, vertices[0].z);
    Vector maxVertex(vertices[0].x, vertices[0].y, vertices[0].z);

    for (int i = 1; i < 8; i++) {
        minVertex.x = std::min(minVertex.x, vertices[i].x);
        minVertex.y = std::min(minVertex.y, vertices[i].y);
        minVertex.z = std::min(minVertex.z, vertices[i].z);

        maxVertex.x = std::max(maxVertex.x, vertices[i].x);
        maxVertex.y = std::max(maxVertex.y, vertices[i].y);
        maxVertex.z = std::max(maxVertex.z, vertices[i].z);
    }

    return Point(0.5f * (minVertex + maxVertex));
}

int CubeMesh::FaceCount(void)
{
    return 12;
}

Mesh::Face CubeMesh::GetFace(int i)
{
    return faces[i];
}

int CubeMesh::EdgeCount(void)
{
	return 12;
}

Mesh::Edge CubeMesh::GetEdge(int i)
{
    return edges[i];
}
