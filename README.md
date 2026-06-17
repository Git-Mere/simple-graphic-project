# Simple Graphic Engine
A software + hardware rendering study covering the full 3D graphics pipeline, written in C++ and OpenGL.

![banner](s_banner.png)

## Features

- **2D/3D Affine Transformations** — translate, rotate, and scale matrices implemented from scratch, verified against a dedicated black-box test suite before use in any draw call
- **Software Triangle Rasterizer** — scanline fill with perspective-correct attribute interpolation and a depth buffer for accurate z-ordering
- **Camera & Frustum System** — view/projection matrices, frustum mesh visualization, and half-space clipping for geometry that straddles the view volume boundary
- **Texture Mapping** — BMP/PNG texture loading with UV-mapped rendering across both the software and GPU-side paths
- **Progressive Tutorial Series** — six self-contained projects (`tutorial-0` through `tutorial-5`) tracing the pipeline from an empty window to a lit, textured scene

## Demo

![demo](https://piskel-imgstore-b.appspot.com/img/dc08376e-ec08-11ef-87fa-b3fea4fe7937.gif)

## Built With

- **C++** — core implementation language; all math (vectors, matrices, rasterizer) written by hand to understand the underlying mechanics directly
- **OpenGL / GLSL** — GPU-side rendering pipeline; GLSL shaders handle vertex transforms and fragment output on the hardware path
- **GLEW** — resolves OpenGL extension function pointers at runtime on Windows without manual loading boilerplate
- **SDL2** — cross-platform window creation and input handling, keeping GL context setup self-contained
- **ImGui** — immediate-mode debug UI for tweaking camera and transform parameters at runtime without rebuilding
- **stb_image** — single-header image decoder for BMP and PNG textures, eliminating a heavier image-loading dependency

## Getting Started

### Prerequisites

- Windows 10 or later
- Visual Studio 2019 or 2022 (C++17 toolset)
- All external libraries (SDL2, GLEW, ImGui, stb_image) are pre-bundled under `cs200-opengl-dev-Seungheon-digipen-master/external/`

### Installation

```bash
git clone https://github.com/Git-Mere/simple-graphic-project.git
```

1. Open `cs200-opengl-dev-Seungheon-digipen-master/opengl-dev.sln` in Visual Studio.
2. In Solution Explorer, select the target project (e.g. `camera_frustum` or any `tutorial-N`).
3. Right-click → **Set as Startup Project**, then press **F5** to build and run.

The required runtime DLLs (`SDL2.dll`, `glew32.dll`) are already placed alongside the prebuilt executables in each project's `Demo/` folder.

## What I Learned

**Problem:** A 3D rendering pipeline has at least two failure surfaces — the math layer and the draw layer — and a visual artifact in the output rarely points clearly at which one is broken.

**Approach:** I separated concerns into explicit phases. All affine and projection math (`Affine.cpp`, `Projection.cpp`) was written first and tested against a dedicated black-box suite (`AffineTest.cpp`, `VectorTest.lib`) before a single draw call was involved. Isolating correctness at the math layer meant that when geometry appeared wrong on screen, the cause was in the rasterizer or pipeline wiring, not in the matrices.

The software rasterizer (`RasterUtilities.cpp`) made perspective-correct interpolation concrete rather than abstract. Naïvely interpolating UV coordinates in screen space produces visible warping on any quad that is not perpendicular to the camera. Working through the fix — dividing each attribute by `w` before interpolation and recovering it per pixel — turned a formula from a textbook into a visible before/after result.

The frustum clipping module (`Clip.cpp`, `HalfSpace.cpp`) added a further layer of edge cases: a triangle straddling a clip plane must be split into one or two new triangles, each with correctly re-interpolated attributes. Handling the three vertex-inside configurations (one, two, zero) clarified why a GPU's fixed-function clipper is a non-trivial piece of work.

**Result:** By the `camera_frustum` module, the pipeline handled textured, depth-tested geometry under a movable camera — first in software, then with the same transforms handed off to GLSL shaders on the GPU path — with each subsystem independently validated.

## License

No license is specified. All rights reserved by the author.