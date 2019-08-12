# Computer Graphics With OpenGL

Adapted from Joey de Vries - Learn OpenGL and David Wolff - OpenGL 4.0 Shading Language Cookbook.

### Getting Started

The project is configured for Mac OSX using [CMake](https://cmake.org/download/).

1) Download and install CMake.app
2) Open Terminal.app on your Mac
2) Install CMake Command Line Tools for terminal:
```
$ sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
```
3) Locate project root directory and ensure that you have the 'build', 'src', 'Includes', 
'Libraries' folders, including 'CMakeLists.txt' and ComputerGraphicsWithOpenGLConfig.h.in files.
```
$ cd ./Downlads/ComputerGraphicsWithOpenGL
$ ls  

CMakeLists.txt				Libraries
README.md
ComputerGraphicsWithOpenGLConfig.h.in	build
Includes				src
```
4) Check that cmake is running in Terminal.
```
$ cmake --version
cmake version 3.7.2

$ which cmake   
/usr/local/bin/cmake
```

5) Go to the build folder
```
$ cd ./build
```

6) Configure and generate the Xcode project inside the build folder
```
$ cmake -G "Xcode" ../
$ ls
CMakeCache.txt				ComputerGraphicsWithOpenGLConfig.h
CMakeFiles				cmake_install.cmake
CMakeScripts				src
ComputerGraphicsWithOpenGL.xcodeproj
```

7) The Xcode project should be available in the build folder of the project. 
Open the Xcode project `ComputerGraphicsWithOpenGL.xcodeproj` and set the active scheme to ComputerGraphicsWithOpenGL target in the Build Scheme.
```
Product => Scheme => Choose Scheme => ComputerGraphicsWithOpenGL
```

8) Build and Run ComputerGraphicsWithOpenGL
```
Product => Run
```



## Controls

### Move Camera: 
* Move in the scene with **Arrow Keys** :arrow_right:    :arrow_left:   :arrow_up:   :arrow_down:  



### Move Pointlights: 
* Move pointlight Up with ***Q***
* Move pointlight Down with ***A***
* Move pointlight Forward with ***S***
* Move pointlight Backward with ***D***
* Move pointlight Right with ***E***
* Move pointlight Left with ***W***
* Select pointlight with ***I***



### Look Around:
* **Move and Hold** screen with :computer_mouse:

* **Zoom** with :computer_mouse: (Mouse wheel)



### UI Interaction:
* Click on **Buttons** or drag **Sliders** with :computer_mouse:




## Content

### Objects
* Cube
* Sphere
* Quad
* Torus
* Torus Knot
* Metal Balls
* Teapot
* Planar Terrain and HeightMap Terrain
* Trolley

### Lighting 
* Blinn Phong Lighting
* HDR Tone Mapping
* Image Based Lighting (IBL)

### Shaders
* Bump mapping
* Normal Mapping
* Parallax Mapping
* Chromatic Aberration
* Environment Mapping (reflection and refraction)
* Disintergration
* Explosion Fireball (Geometry Shader)
* Wireframe (Geometry Shader)
* Porcupine / Display Normal (Geometry Shader)
* Toon / Cell shading
* Physically Based Rendering (PBR)
* Fast Approximate Anti-Aliasing (FXAA)

### Post Processing Effects
* Color Inversion
* GrayScale
* Kernel
* Kernel Blur
* Sobel Edge Detection
* Frei-Chen Edge Detection
* Screen Wave
* Swirl
* Night Vision
* Lens Circle
* Posterization
* Dream Vision
* Pixelate
* Pixelation
* Knitted Pixelation
* Frosted Glass 1
* Frosted Glass 2
* Crosshatching
* Predators Thermal Vision
* Toonify
* Shockwave
* Fish Eye
* Barrel Distortion
* MultiScreen Fish Eye
* Fish Eye Lens
* Fish Eye / Anti-Fish Eye
* Gaussian Blur
* Blur
* Radial Blur
* Motion Blur
* Vignetting
* Bright Parts
* Bloom
* Lens Flare
* Screen Space Ambient Occlusion
* Depth Testing
* Depth Mapping
* Directional Shadow Mapping
* Omnidirectional Shadow Mapping
* Deferred Rendering
* Rain Drops
* Palette Quantization And Dithering
* Distorted TV
* RGB Display
* Retro Parallax
* Money Filter
* Microprism Mosaic
* Bayer Matrix Dithering
* Julia Freak
* Heart Blend
* EM Interference
* Cubic Lens Distortion
* Cel Shaderish
* Cartoon Video


## Built With

* [Assimp](http://assimp.org/)
* [FMOD](https://www.fmod.com/)
* [FreeImage](http://freeimage.sourceforge.net/download.html)
* [FreeType](https://www.freetype.org/)
* [The OpenGL Extension Wrangler Library (GLEW)](http://glew.sourceforge.net/)
* [OpenGL Mathematics (GLM)](https://glm.g-truc.net/0.9.2/api/index.html)
* [GLFW](https://www.glfw.org/)

## Acknowledgments

* Joey de Vries - [Learn OpenGL](https://learnopengl.com/Introduction "Learn OpenGL Introduction").
* David Wolff - [OpenGL 4.0 Shading Language Cookbook](https://github.com/daw42/glslcookbook "David Wolff OpenGL 4.0 Shading Language Cookbook").
