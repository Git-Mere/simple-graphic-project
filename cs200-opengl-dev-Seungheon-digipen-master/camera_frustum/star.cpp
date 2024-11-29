#include "star.h"

namespace {
    float frand(float a = 0, float b = 1) {
        return a + (b - a) * float(rand()) / float(RAND_MAX);
    }
    const float PI = 4.0f * atan(1.0f);
}

star::star(Mesh& m, const Vector& position, float scale_size) : mesh_ptr(&m), scale_size(scale_size), position(position)
{
    Vector col(frand(0.7f), frand(0.7f), frand(0.7f));
    color = col;
    to_world = translate(position) * scale(scale_size);
}


void star::draw(const Matrix& world_to_clip, Render& r)
{   
    int vertex_size = mesh_ptr->VertexCount();
    int face_size = mesh_ptr->FaceCount();
    temp_clip_verts.clear();
    r.SetColor(color);

    for (int j = 0; j < vertex_size; ++j) {
        temp_clip_verts.push_back((world_to_clip * to_world) * mesh_ptr->GetVertex(j));
    }
    for (int j = 0; j < face_size; ++j) {
        const Mesh::Face& face = mesh_ptr->GetFace(j);
        const Hcoord& Pclip = temp_clip_verts[face.index1],
            & Qclip = temp_clip_verts[face.index2],
            & Rclip = temp_clip_verts[face.index3];        
        r.FillTriangle(Pclip, Qclip, Rclip);
    }
    
}

