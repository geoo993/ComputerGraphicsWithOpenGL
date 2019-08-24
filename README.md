# Computer Graphics With OpenGL

This project was inspired and adapted from [Joey de Vries](https://learnopengl.com/Introduction "Learn OpenGL Introduction") and [David Wolff](https://github.com/daw42/glslcookbook "David Wolff OpenGL 4.0 Shading Language Cookbook"). Computer graphics is a field in Computer Science which studies methods for digitally synthesizing and manipulating visual content to make computers easier to interact with, and better for understanding and interpreting many types of data. Computer graphics development has had a profound impact on many types of media and have revolutionised movies, medical imaging, product design, and the computer game industry.

This project aims to illustrate some of the most common types of computer graphics techniques created using c++ and OpenGL. These include primitive rendering, camera system, lighting, materials, texturing, fragment and geometry shading, and post processing filters.


## Getting Started

The project is configured for Mac OSX using [CMake](https://cmake.org/download/).

1) Download and install ```CMake.app```
2) Open ```Terminal.app``` on your Mac
2) Install CMake Command Line Tools for Terminal
```
$ sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
```
3) Locate the project root directory (Where you Downloaded the project) and ensure 
that you have the 'build', 'src', 'Includes', 'Libraries' folders, 
including 'CMakeLists.txt' and ComputerGraphicsWithOpenGLConfig.h.in files.
```
$ cd ./Downloads/ComputerGraphicsWithOpenGL
$ ls  

CMakeLists.txt				Libraries
README.md
ComputerGraphicsWithOpenGLConfig.h.in	build
Includes				src
```
NOTE: Please fetch all the large files from ```$ cd src/resources``` using git lfs
```
$ git lfs install
$ git lfs pull
Downloading LFS objects:  53% (101/190), 198 MB | 2.7 MB/s
```

4) Check all the project libraries are available
```
$ cd Libraries 
$ ls
libGLEW.2.1.0.dylib           libfmodstudioL.dylib
libassimp.4.1.0.dylib         libfreeimage.3.17.0.dylib
libfmod.dylib                 libfreeimageplus.3.17.0.dylib
libfmodL.dylib                libfreetype.6.dylib
libfmodstudio.dylib           libglfw.3.2.dylib
```

5) Download the missing ```libpng16.16.dylib``` shared library of ```libfreetype.6.dylib```
```
$ brew install libpng
🍺  /usr/local/Cellar/libpng/1.6.37: 27 files, 1.2MB
```

6) Check that cmake is running in Terminal
```
$ cmake --version
cmake version 3.7.2

$ which cmake   
/usr/local/bin/cmake
```

7) Go to the build folder
```
$ cd ../build
```

8) Configure and generate the Xcode project inside the build folder
```
$ cmake -G "Xcode" ../
$ ls
CMakeCache.txt				ComputerGraphicsWithOpenGLConfig.h
CMakeFiles				cmake_install.cmake
CMakeScripts				src
ComputerGraphicsWithOpenGL.xcodeproj
```

9) The Xcode project should be available in the build folder of the project. 
Open the Xcode project ```ComputerGraphicsWithOpenGL.xcodeproj``` and set the active scheme to ComputerGraphicsWithOpenGL target in the Build Scheme
```
Product => Scheme => Choose Scheme => ComputerGraphicsWithOpenGL
```

10) Build and Run ComputerGraphicsWithOpenGL in Xcode
```
Product => Run
```
or use Terminal 
```
$ cmake --build . --target ComputerGraphicsWithOpenGL --config Debug
$ ./src/Debug/ComputerGraphicsWithOpenGL
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
<p align="left"> Image Based Lighting (IBL) </p>
<p align="left"> <img src="images/IBL.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Blinn Phong Lighting </p>
<p align="right"><img src="images/BlinnPhong.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> HDR Tone Mapping </p>
<p align="left"> <img src="images/None.png" width="960px" height="540px" /> </p>
<br/>
  
### Shaders
<p align="left"> Physically Based Rendering (PBR) </p>
<p align="left"><img src="images/PBR.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Bump Mapping </p>
<p align="right"> <img src="images/BumpMapping.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Parallax Mapping </p>
<p align="left"><img src="images/ParallaxMapping.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Chromatic Aberration </p>
<p align="right"> <img src="images/ChromaticAberration.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Environment Mapping (Reflection and Refraction) </p>
<p align="left"><img src="images/EnvironmentMapping.png" width="960px" height="540px" /> </p>
<br/>
<p align="right">  Disintergration </p>
<p align="right"> <img src="images/Disintergration.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Fireball (Geometry Shader) </p>
<p align="left"><img src="images/Fireball.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Wireframe (Geometry Shader) </p>
<p align="right"> <img src="images/Wireframe.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Porcupine / Display Normal (Geometry Shader) </p>
<p align="left"><img src="images/Porcupine.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Toon / Cell shading </p>
<p align="right"> <img src="images/Toon.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Fast Approximate Anti-Aliasing (FXAA) (Geometry Shader) </p>
<p align="left"><img src="images/Faaa.png" width="960px" height="540px" /> </p>
<br/>

### Post Processing Effects
<p align="left"> Color Inversion </p>
<p align="left"> <img src="images/ColorInversion.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Gray Scale </p>
<p align="right"><img src="images/Grayscale.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Kernel </p>
<p align="left"> <img src="images/Kernel.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Kernel Blur </p>
<p align="right"><img src="images/KernelBlur.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Sobel Edge Detection </p>
<p align="left"> <img src="images/SobelEdgeDetection.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Frei-Chen Edge Detection </p>
<p align="right"><img src="images/FreiCheinEdgeDetection.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Screen Wave </p>
<p align="left"> <img src="images/ScreenWave.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Swirl </p>
<p align="right"><img src="images/Swirl.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Night Vision </p>
<p align="left"> <img src="images/NightVision.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Lens Circle </p>
<p align="right"><img src="images/LensCircle.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Posterization </p>
<p align="left"> <img src="images/Posterization.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Dream Vision </p>
<p align="right"><img src="images/DreamVision.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Pixelate </p>
<p align="left"> <img src="images/Pixelate.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Pixelation </p>
<p align="right"><img src="images/Pixelation.png" width="960px" height="540px" /> </p>
<br/>
<p align="left"> Knitted Pixelation </p>
<p align="left"> <img src="images/KnittedPixelation.png" width="960px" height="540px" /> </p>
<br/>
<p align="right"> Frosted Glass 1 </p>
<p align="right"><img src="images/None.png" width="960px" height="540px" /> </p>
<br/>

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
* [Geeks3D](https://www.geeks3d.com)
* [Shadertoy](https://www.shadertoy.com)
