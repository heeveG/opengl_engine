# OpenGL Renderer

### Description
The project was written using CMake 3.17 as the building tool, OpenGL 4.2 as the Graphics API, Microsoft Visual Studio 2019 as an IDE, and a C++17 standard.

Different algorithms for masking the blur used in the application:
* Branch (a simple if-statement)
* Texture mask (a black-white image used as a mask in the shader)
* Scissor (`glScissor`: only pixels that lie within the specified rectangle can be modified by drawing commands)

Illumination models used:
* Blinn-Phong
* PBR using Cook-Torrance BRDF
    * Diffuse: Lambertian
    * Specular: Cook-Torrance 
    * Fresnel term : Schlick
    * Microfacet distribution: GGX
    * Geometry attenuation: GGX-Smith

Post-processing:
* Gamma correction
* HDR Reinhard tonemapping

### Building the project:
From the root directory:

```
mkdir build && cd build
cmake .. -G <generator> // Specify a build system generator for your platform.
cmake --build . // Or other way to build the project (MVS Solution, Xcode...).
```

### Dependencies
All relevant libraries can be found in `/external_sources` and will be built as a project dependencies.
* Window & Input system: GLFW
* OpenGL Function Pointer loader: GLEW
* OpenGL maths: GLM
* Image loading: stb_image
* Model loading: Assimp

### Usage
After the app has run:

* press `B` to toggle the Blur
* press `Q` to enable Blinn-Phong lighting model
* press `W` to enable PBR using Cook-Torrance BRDF lighting model

To change the algorithm for masking the blur set one of the following definitions: `BLUR_SCISSOR`, `BLUR_MASK`, `BLUR_BRANCH` in the CMake file and rebuild the project.
