#pragma once
#include <vector>
#include "Mesh.h"
#include "Affine.h"
#include "Render.h"
#include "CameraRender2D.h"

class planet {
public:
    planet(Mesh& m, const Vector& color, float r, float speed, float scale = 1, Vector rotate_axis = Vector(0,1,0));
    void update(float time);
    void draw(const Matrix& world_to_cam, const Matrix& cam_to_clip, Render& r);
    void draw_orbit(CameraRender2D& d);
    const Affine& Get_to_world();

private:
    std::vector<Hcoord> orbit;
    Mesh* mesh_ptr;
    Affine to_world;
    Vector color;
    Vector position;
    float orbit_radius = 0;
    float speed = 0;
    float rotate_speed = 2 * PI / 3.0f;
    float scale_size = 1;
    Affine orbit_matrix = scale(1.f);
    Vector orbit_color = Vector(1, 1, 1);

    const float PI = 4.0f * atan(1.0f);
    const Point O = Point(0, 0, 0);
    const int orbit_vertices = 100;
    const Vector rotate_axis = Vector(0,1,0);

    std::vector<Hcoord> temp_cam_verts;
    std::vector<Hcoord> temp_clip_verts;

};