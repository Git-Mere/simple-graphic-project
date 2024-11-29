/*!************************************************************************
\file FrustumMesh.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Camera Frustum Programming Assignment
\date 11-10-2023
\brief
Implement function about SimpleRender
**************************************************************************/

#include "FrustumMesh.h"
#include <iostream>
const Mesh::Face FrustumMesh::faces[12] = {
    Mesh::Face(0, 2, 1),
    Mesh::Face(0, 3, 2),
    Mesh::Face(4, 5, 6),
    Mesh::Face(4, 6, 7),
    Mesh::Face(0, 1, 5),
    Mesh::Face(0, 5, 4),
    Mesh::Face(1, 2, 6),
    Mesh::Face(1, 6, 5),
    Mesh::Face(2, 3, 7),
    Mesh::Face(2, 7, 6),
    Mesh::Face(3, 0, 4),
    Mesh::Face(3, 4, 7)
};
const Mesh::Edge FrustumMesh::edges[16] = {
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
    Mesh::Edge(3, 7),
    Mesh::Edge(4, 8),
    Mesh::Edge(5, 8),
    Mesh::Edge(6, 8),
    Mesh::Edge(7, 8)
};

/*!*****************************************************************************
\brief
    this constructor take fov = angle, a = aspect, n = near, f = far. based on this data, vertices is determined
\param fov
    angle
\param a
    aspect ratio
\param n
    near
\param f
    far
*******************************************************************************/
FrustumMesh::FrustumMesh(float fov, float a, float n, float f)
{
    float n_width = 2 * std::tan(fov / 2) * n ;
    float n_height = n_width / a;

    float f_width = 2 * std::tan(fov / 2) * f;
    float f_height = f_width / a;

    vertices[0] = Point(-f_width / 2, f_height / 2, -f);
    vertices[1] = Point(-f_width / 2, -f_height / 2, -f);
    vertices[2] = Point(f_width / 2, -f_height / 2, -f);
    vertices[3] = Point(f_width / 2, f_height / 2, -f);
    vertices[4] = Point(-n_width / 2, n_height / 2, -n);
    vertices[5] = Point(-n_width / 2, -n_height / 2, -n);
    vertices[6] = Point(n_width / 2, -n_height / 2, -n);
    vertices[7] = Point(n_width / 2, n_height / 2, -n);
    vertices[8] = Point();
}

int FrustumMesh::VertexCount(void)
{
    return 9;
}

Point FrustumMesh::GetVertex(int i)
{
    return vertices[i];
}

Vector FrustumMesh::Dimensions(void)
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

Point FrustumMesh::Center(void)
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
    Vector sum = 0.5f * (minVertex + maxVertex);
    Point result(sum.x, sum.y, sum.z);
    return result;
}

int FrustumMesh::FaceCount(void)
{
    return 12;
}

Mesh::Face FrustumMesh::GetFace(int i)
{
    return faces[i];
}

int FrustumMesh::EdgeCount(void)
{
    return 16;
}

Mesh::Edge FrustumMesh::GetEdge(int i)
{
    return edges[i];
}
