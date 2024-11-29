#include "planet.h"

planet::planet(Mesh& m, const Vector& color, float r, float speed, float scale, Vector rotate_axis)
	: mesh_ptr(&m), color(color), orbit_radius(r), speed(speed) , scale_size(scale), rotate_axis(rotate_axis)
{
    for (float t = 0; t < orbit_vertices; ++t) {
        float theta = (2.0f * PI * t) / static_cast<float>(orbit_vertices);
        float x = orbit_radius * cos(theta);
        float y = 0;
        float z = orbit_radius * sin(theta);
        Hcoord objectCoord = Point(x, y, z);
        orbit.push_back(objectCoord);
    }
}

void planet::update(float time)
{
    position = Point(orbit_radius * cos(speed * PI * time / 15.0f), 0, orbit_radius * sin(speed * PI * time / 15.0f)) - O;

    to_world = translate(position) * rotate(rotate_speed * time, rotate_axis)
        * scale(scale_size);
}

void planet::draw(const Matrix& world_to_cam, const Matrix& cam_to_clip, Render& r)
{
    int vertex_size = mesh_ptr->VertexCount();
    int face_size = mesh_ptr->FaceCount();

    Affine obj_to_cam = world_to_cam * to_world;
    temp_cam_verts.clear();
    temp_clip_verts.clear();
    for (int j = 0; j < vertex_size; ++j) {
        temp_cam_verts.push_back(obj_to_cam * mesh_ptr->GetVertex(j));
        temp_clip_verts.push_back(cam_to_clip * temp_cam_verts[j]);
    }
    for (int j = 0; j < face_size; ++j) {
        const Mesh::Face& face = mesh_ptr->GetFace(j);
        const Point& Pcam = temp_cam_verts[face.index1],
            & Qcam = temp_cam_verts[face.index2],
            & Rcam = temp_cam_verts[face.index3];
        const Hcoord& Pclip = temp_clip_verts[face.index1],
            & Qclip = temp_clip_verts[face.index2],
            & Rclip = temp_clip_verts[face.index3];


        Vector normal = normalize(cross(Qcam - Pcam, Rcam - Pcam));
        float mu = normal.z;
        if (orbit_radius == 0) {
            mu = 1;
        }
        r.SetColor(mu * color);
        r.FillTriangle(Pclip, Qclip, Rclip);

    }

}

void planet::draw_orbit(CameraRender2D& d)
{
    if (orbit_radius != 0) {
        d.DisplayEdges(orbit, orbit_matrix, orbit_color);
    }
}

const Affine& planet::Get_to_world()
{
    return to_world;
}

