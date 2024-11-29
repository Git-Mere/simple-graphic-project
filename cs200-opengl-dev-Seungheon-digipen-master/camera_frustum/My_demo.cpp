// CollisionDemo.cpp
// -- collision detection using point + polyhedron test
// cs250 7/17
//
// To compile/link from the Visual Studio command prompt:
//     cl /EHsc CollisionDemo.cpp Affine.lib Camera.lib Projection.lib
//        CubeMesh.lib SnubDodecMesh.cpp Render.lib HalfSpace.cpp opengl32.lib
//        glew32.lib SDL2.lib SDL2main.lib /link /subsystem:console
// To compile/link from the Linux command line:
//     g++ CollisionDemo.cpp Affine.cpp Camera.cpp Projection.cpp
//         CubeMesh.cpp SnubDodecMesh.cpp Render.cpp HalfSpace.cpp
//         -lGL -lGLEW -lSDL2

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "CubeMesh.h"
#include "SnubDodecMesh.h"
#include "Projection.h"
#include "Render.h"
#include "CameraRender2D.h"
#include "planet.h"
#include "star.h"
using namespace std;

#undef main
const float PI = 4.0f * atan(1.0f);
const float TWO_PI = 8.0f * atan(1.0f);
const Point O(0, 0, 0);
const Vector EX(1, 0, 0),
EY(0, 1, 0),
EZ(0, 0, 1);
const Vector WHITE(1, 1, 1),
BLACK(0, 0, 0),
PURPLE(1, 0, 1),
BLUE(0, 0, 1),
GREEN(0, 1, 0),
RED(1, 0, 0),
Mer(0.55f, 0.54f, 0.55f),
Ven(0.69f, 0.44f, 0.22f),
Earth(0.258f, 0.607f, 0.854f),
Mar(0.741f, 0.419f, 0.231f),
Jup(0.968f, 0.901f, 0.784f),
Sat(0.686f, 0.643f, 0.513f),
Ura(0.682f, 0.854f, 0.949f),
Nep(0.384f, 0.521f, 0.733f);
constexpr int window_width = 600;
constexpr int window_height = 600;
constexpr int orbit_vertices = 100;
constexpr int star_num = 30;
constexpr float ring = 10;
const float for_ring = TWO_PI / ring;


class Client {
public:
    Client(void);
    ~Client(void);
    void draw(float dt);
    void moveCamera(SDL_Keycode kc);
    void DisplayEdges(Mesh& mesh, const Affine& obj_to_world, const Vector& clr);
    void mousemove(int x_dir, int y_dir);
private:
    Camera cam, cam_0;
    CubeMesh cube;
    SnubDodecMesh snub;
    Render render;
    CameraRender2D d2{render};
    float time;
    vector<planet> planets;
    vector<star> stars;
    std::vector<Hcoord> saturn_ring;
    
};

namespace {
    float frand(float a = 0, float b = 1) {
        return a + (b - a) * float(rand()) / float(RAND_MAX);
    }
}

Client::Client(void) {
    cam = Camera(O + 10 * EZ + 10 * EY, -EZ, EY, 0.5f * PI, 1, 0.1f, 200);
    cam_0 = cam;
    d2.SetCamera(cam);
    time = 1;

    float planet_size[] = { 4.f, 0.5f, 1.f ,1.f, 0.7f, 2.5f, 3.f, 1.5f, 1.5f};

    planet sun(snub, RED, 0, 0, planet_size[0]);
    planet mercury(snub, Mer, 6, 8, planet_size[1]);
    planet venus(snub, Ven, 8, 6, planet_size[2]);
    planet earth(snub, Earth, 10, 5, planet_size[3]);
    planet mars(snub, Mar, 12, 4, planet_size[4]);
    planet jupiter(snub, Jup, 22, 4, planet_size[5]);
    planet saturn(snub, Sat, 30, 3, planet_size[6], Vector(-1, 1, 0));
    planet uranos(snub, Ura, 34, 2, planet_size[7]);
    planet neptune(snub, Nep, 38, 2.5f, planet_size[8]);

    planets.push_back(sun);
    planets.push_back(mercury);
    planets.push_back(venus);
    planets.push_back(earth);
    planets.push_back(mars);
    planets.push_back(jupiter);
    planets.push_back(saturn);
    planets.push_back(uranos);
    planets.push_back(neptune);

    star star1;
    Vector pos;
    for (int i = 0; i < star_num; i++) {
        if (i < 10) {
            pos = Vector(frand(-100, 100), frand(50, 100), frand(-100, 100));
        }
        else if (i >= 10 && i < 20) {
            pos = Vector(frand(-100, 100), frand(-50, -100), frand(-100, 100));
        }
        else if (i >= 20 && i < 25) {
            pos = Vector(frand(-100, 100), frand(-20, 20), frand(-100, -200));
        }
        else if (i >= 25 && i < 30) {
            pos = Vector(frand(-100, 100), frand(-20, 20), frand(100, 200));
        }
        float scale = frand(0.2f);
        star1 = star(cube, pos, scale);
        stars.push_back(star1);

    }
}


Client::~Client(void) { }


void Client::draw(float dt) {
    render.ClearBuffers(BLACK);
    Affine world_to_cam = WorldToCamera(cam);
    Matrix cam_to_clip = CameraToNDC(cam);
    Matrix world_to_clip = cam_to_clip * world_to_cam;
    
    // render planet
    for (unsigned i = 0; i < planets.size(); ++i) {
        planets[i].update(time);
        planets[i].draw(world_to_cam, cam_to_clip, render);
        planets[i].draw_orbit(d2);
    }

    //satrun ring
    for (float i = 1.2f; i < 1.5f; i += 0.05f) {
        saturn_ring.clear();
        for (float t = 0; t < ring; ++t) {
            float theta = for_ring * t;
            float x = i * cos(theta);
            float y = i * sin(theta);
            float z = i * sin(theta);
            Hcoord objectCoord = Point(x, y, z);
            saturn_ring.push_back(objectCoord);
        }
        d2.DisplayEdges(saturn_ring, planets[6].Get_to_world(), Sat);
    }

    //render star
    for (unsigned i = 0; i < stars.size(); ++i) {       
        stars[i].draw(world_to_clip, render);
    }
    d2.SetCamera(cam);


    time += dt;
}

void Client::mousemove([[maybe_unused]]int x_dir, [[maybe_unused]] int y_dir) {
    float r1 = PI / 7200;

    float yaw_angle = -r1 * x_dir;
    float pitch_angle = -r1 * y_dir;

    cam.Yaw(yaw_angle);
    cam.Pitch(pitch_angle);
}

void Client::moveCamera(SDL_Keycode kc) {

    const float dist_increment = 0.1f;
    //if (kc == SDLK_a) {
    //    cam.MoveLeft(dist_increment);
    //}
    //if (kc == SDLK_d) {
    //    cam.MoveRight(dist_increment);
    //}
    if (kc == SDLK_w) {
        cam.Forward(dist_increment);
    }
    if (kc == SDLK_s) {
        cam.Forward(-dist_increment);
    }
    if (kc == SDLK_r) {
        cam = cam_0;
    }

}

/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int /*argc*/, char* /*argv*/[]) {
    srand(unsigned(time(nullptr)));
    // SDL: initialize and create a window
    SDL_Init(SDL_INIT_VIDEO);
    const char* title = "CS 250: Collision Demo";

    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, window_width, window_height,
        SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // GLEW: get function bindings (if possible)
    glewInit();
    if (!GLEW_VERSION_2_0) {
        cout << "needs OpenGL version 2.0 or better" << endl;
        return -1;
    }

    // animation loop
    try {
        bool done = false;
        Client* client = new Client();
        Uint32 ticks_last = SDL_GetTicks();
        while (!done) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    else
                        client->moveCamera(event.key.keysym.sym);
                    break;
                case SDL_MOUSEMOTION:
                    client->mousemove(event.motion.xrel, event.motion.yrel);
                    break;
                }

            }
            Uint32 ticks = SDL_GetTicks();
            float dt = 0.001f * (ticks - ticks_last);
            ticks_last = ticks;
            client->draw(dt);
            SDL_GL_SwapWindow(window);
        }

        delete client;
    }

    catch (exception& e) {
        cout << e.what() << endl;
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}

