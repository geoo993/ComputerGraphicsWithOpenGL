# Computer Graphics With OpenGL

This project was inspired [Joey de Vries - Learn OpenGL](https://learnopengl.com/Introduction "Learn OpenGL Introduction") and [David Wolff - OpenGL 4.0 Shading Language Cookbook](https://github.com/daw42/glslcookbook "David Wolff - OpenGL 4.0 Shading Language Cookbook"). It will illustrate some of the most common computer graphics techinques used in the industry, including topics such as primitive objects, camera system, texturing, lighting, fragment and geometry shading, shadows and screen-space filters.

[Computer graphics](https://en.wikipedia.org/wiki/Computer_graphics "Computer Graphics") is a field in Computer Science which studies methods for digitally synthesizing and manipulating visual content to make computers ```easier``` to interact with, and ```better``` for understanding and interpreting many types of data. It has profound impact in many types of media (graphics design, animation, 3d modelling, medical imaging, image processing, and computer vision among others) and industries (films, television, advertising and video games).


## Getting Started

NOTE: The project is configured for Mac OSX using [CMake](https://cmake.org/download/).

1) Download and install the [CMake.app](https://cmake.org/download/) for ```Mac OS X 10.7 or later```
2) Open ```Terminal.app``` on your Mac and enter the following commands.
2) Install ```CMake Command Line Tools``` for Terminal
```
% sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
```
3) [Clone](https://github.com/geoo993/ComputerGraphicsWithOpenGL) the project on your Mac and ensure 
that you have the 'build', 'src', 'Includes', 'Libraries' folders, 
including 'CMakeLists.txt' and ComputerGraphicsWithOpenGLConfig.h.in files.
```
% cd ./Downloads
% git clone https://github.com/geoo993/ComputerGraphicsWithOpenGL.git
% cd ./ComputerGraphicsWithOpenGL
% ls  

CMakeLists.txt				Libraries
README.md
ComputerGraphicsWithOpenGLConfig.h.in	build
Includes				src
```
NOTE: Please fetch all the large files from the resources directory using git lfs
```
% cd ./src/resources
% brew install git lfs
% git lfs install
% git lfs pull
Downloading LFS objects:  53% (101/190), 198 MB | 2.7 MB/s
```

4) Check that all the project libraries are available
```
% cd ../../Libraries 
% ls
libGLEW.2.1.0.dylib           libfmodstudioL.dylib
libassimp.4.1.0.dylib         libfreeimage.3.17.0.dylib
libfmod.dylib                 libfreeimageplus.3.17.0.dylib
libfmodL.dylib                libfreetype.6.dylib
libfmodstudio.dylib           libglfw.3.2.dylib
```

5) Download the missing ```libpng16.16.dylib``` shared library of ```libfreetype.6.dylib``` with [Homebrew](https://brew.sh/)
```
% brew install libpng
🍺  /usr/local/Cellar/libpng/1.6.37: 27 files, 1.2MB
```

6) Check that cmake is running in Terminal
```
% cmake --version
cmake version 3.15.2

% which cmake   
/usr/local/bin/cmake
```

7) Go to the ```build``` folder
```
% cd ../build
```
and make sure you have installed ```Xcode Command Line Tools``` and pointing towards the Xcode `Developer` directory
```
% xcode-select --install # Install Command Line Tools if you haven't already.
% sudo xcode-select --switch /Library/Developer/CommandLineTools # Enable command line tools
```
or alternatively specify to use the full Xcode.app (if you have it installed) with:
```
% sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
```

8) Configure and generate the Xcode project inside the ```build``` folder
```
% cmake -G "Xcode" ../
% ls
CMakeCache.txt				ComputerGraphicsWithOpenGLConfig.h
CMakeFiles				cmake_install.cmake
CMakeScripts				src
ComputerGraphicsWithOpenGL.xcodeproj
```

9) The Xcode project should be available in the ```build``` folder of the project. 
Open the Xcode project ```ComputerGraphicsWithOpenGL.xcodeproj``` and set the active scheme to ```ComputerGraphicsWithOpenGL``` target in the ```Build Scheme```
```
Find and select: Product => Scheme => Choose Scheme => ComputerGraphicsWithOpenGL
```

10) Build and Run ComputerGraphicsWithOpenGL in Xcode
```
Find and select: Product => Run
```
or using Terminal 
```
% cmake --build . --target ComputerGraphicsWithOpenGL --config Debug
% ./src/Debug/ComputerGraphicsWithOpenGL
```


## Controls

### Movement: 
* Move in the scene with **Arrow Keys** :arrow_right:    :arrow_left:   :arrow_up:   :arrow_down:  
* Toggle movement speed with ***O***


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
<p align="left"> <a href="https://vimeo.com/355709336"><img src="images/IBL.png" width="960px" height="540px" title="Image Based Lighting (IBL)" alt="Image Based Lighting (IBL)"> </a></p>
<br/>
<p align="right"> Blinn Phong Lighting </p>
<p align="right"> <a href="https://vimeo.com/355708337"><img src="images/BlinnPhong.png" width="960px" height="540px" title="Blinn Phong Lighting" alt="Blinn Phong Lighting"> </a></p>
<br/>
<p align="left"> HDR Tone Mapping </p>
<p align="left"> <a href="https://vimeo.com/355865376"><img src="images/HRDToneMapping.png" width="960px" height="540px" title="HDR Tone Mapping" alt="HDR Tone Mapping"></a> </p>
<br/>
  
### Shaders
<p align="left"> Physically Based Rendering (PBR) </p>
<p align="left"> <a href="https://vimeo.com/355718272"><img src="images/PBR.png" width="960px" height="540px" title="Physically Based Rendering (PBR)" alt="Physically Based Rendering (PBR)"></a> </p>
<br/>
<p align="right"> Bump Mapping </p>
<p align="right"> <a href="https://vimeo.com/geoo993"><img src="images/BumpMapping.png" width="960px" height="540px" title="Bump Mapping" alt="Bump Mapping"></a> </p>
<br/>
<p align="left"> Parallax Mapping </p>
<p align="left"> <a href="https://vimeo.com/geoo993"><img src="images/ParallaxMapping.png" width="960px" height="540px" title="Parallax Mapping" alt="Parallax Mapping"></a> </p>
<br/>
<p align="right"> Chromatic Aberration </p>
<p align="right"> <a href="https://vimeo.com/geoo993"><img src="images/ChromaticAberration.png" width="960px" height="540px" title="Chromatic Aberration" alt="Chromatic Aberration"></a></p>
<br/>
<p align="left"> Environment Mapping (Reflection and Refraction) </p>
<p align="left"> <a href="https://vimeo.com/geoo993"><img src="images/EnvironmentMapping.png" width="960px" height="540px" title="Environment Mapping" alt="Environment Mapping"></a> </p>
<br/>
<p align="right">  Disintergration </p>
<p align="right"> <a href="https://vimeo.com/geoo993"><img src="images/Disintergration.png" width="960px" height="540px" title="Disintergration" alt="Disintergration"></a></p>
<br/>
<p align="left"> Fireball (Geometry Shader) </p>
<p align="left"> <a href="https://vimeo.com/geoo993"><img src="images/Fireball.png" width="960px" height="540px" title="Fireball" alt="Fireball"></a></p>
<br/>
<p align="right"> Wireframe (Geometry Shader) </p>
<p align="right"> <a href="https://vimeo.com/geoo993"><img src="images/Wireframe.png" width="960px" height="540px" title="Wireframe" alt="Wireframe"></a></p>
<br/>
<p align="left"> Porcupine / Display Normal (Geometry Shader) </p>
<p align="left"> <a href="https://vimeo.com/geoo993"><img src="images/Porcupine.png" width="960px" height="540px" title="Porcupine" alt="Porcupine"></a> </p>
<br/>
<p align="right"> Toon / Cell Shading </p>
<p align="right"> <a href="https://vimeo.com/geoo993"><img src="images/Toon.png" width="960px" height="540px" title="Toon Shading" alt="Toon Shading"></a> </p>
<br/>
<p align="left"> Fast Approximate Anti-Aliasing (FXAA) (Geometry Shader) </p>
<p align="left"> <a href="https://vimeo.com/geoo993"><img src="images/Faaa.png" width="960px" height="540px" title="Fast Approximate AntiAliasing" alt="Fast Approximate Anti Aliasing"></a> </p>
<br/>

### Post Processing Effects
<p align="left"> Color Inversion </p>
<p align="left"> <a href="https://vimeo.com/355708630"><img src="images/ColorInversion.png" width="960px" height="540px" title="Color Inversion" alt="Color Inversion"></a> </p>
<br/>
<p align="right"> Gray Scale </p>
<p align="right"> <a href="https://vimeo.com/355709094"><img src="images/Grayscale.png" width="960px" height="540px" title="Gray Scale" alt="Gray Scale"></a> </p>
<br/>
<p align="left"> Kernel </p>
<p align="left"> <a href="https://vimeo.com/355717289"><img src="images/Kernel.png" width="960px" height="540px" title="Kernel" alt="Kernel"></a> </p>
<br/>
<p align="right"> Kernel Blur </p>
<p align="right"> <a href="https://vimeo.com/355717439"><img src="images/KernelBlur.png" width="960px" height="540px" title="Image Based Lighting (IBL)" alt="Image Based Lighting (IBL)"></a> </p>
<br/>
<p align="left"> Sobel Edge Detection </p>
<p align="left"> <a href="https://vimeo.com/355719267"><img src="images/SobelEdgeDetection.png" width="960px" height="540px" title="Kernel Blur" alt="Kernel Blur"></a> </p>
<br/>
<p align="right"> Frei-Chen Edge Detection </p>
<p align="right"> <a href="https://vimeo.com/355708959"><img src="images/FreiCheinEdgeDetection.png" width="960px" height="540px" title="Frei Chen Edge Detection" alt="Frei Chen Edge Detection"></a> </p>
<br/>
<p align="left"> Screen Wave </p>
<p align="left"> <a href="https://vimeo.com/355719082"><img src="images/ScreenWave.png" width="960px" height="540px" title="Screen Wave" alt="Screen Wave"></a> </p>
<br/>
<p align="right"> Swirl </p>
<p align="right"> <a href="https://vimeo.com/355719485"><img src="images/Swirl.png" width="960px" height="540px" title="Swirl" alt="Swirl"></a> </p>
<br/>
<p align="left"> Night Vision </p>
<p align="left"> <a href="https://vimeo.com/355717999"><img src="images/NightVision.png" width="960px" height="540px" title="Night Vision" alt="Night Vision"></a> </p>
<br/>
<p align="right"> Lens Circle </p>
<p align="right"> <a href="https://vimeo.com/355717791"><img src="images/LensCircle.png" width="960px" height="540px" title="Lens Circle" alt="Lens Circle"></a> </p>
<br/>
<p align="left"> Posterization </p>
<p align="left"> <a href="https://vimeo.com/355718993"><img src="images/Posterization.png" width="960px" height="540px" title="Posterization" alt="Posterization"></a> </p>
<br/>
<p align="right"> Dream Vision </p>
<p align="right"> <a href="https://vimeo.com/355708814"><img src="images/DreamVision.png" width="960px" height="540px" title="Dream Vision" alt="Dream Vision"></a> </p>
<br/>
<p align="left"> Pixelate </p>
<p align="left"> <a href="https://vimeo.com/355718657"><img src="images/Pixelate.png" width="960px" height="540px" title="Pixelate" alt="Pixelate"></a> </p>
<br/>
<p align="right"> Pixelation </p>
<p align="right"><a href="https://vimeo.com/355718872"><img src="images/Pixelation.png" width="960px" height="540px" title="Pixelation" alt="Pixelation"></a> </p>
<br/>
<p align="left"> Knitted Pixelation </p>
<p align="left"> <a href="https://vimeo.com/355717607"><img src="images/KnittedPixelation.png" width="960px" height="540px" title="Knitted Pixelation" alt="Knitted Pixelation"></a> </p>
<br/>
<p align="right"> Frosted Glass Pixelation Effect </p>
<p align="right"><a href="https://vimeo.com/355854901"><img src="images/FrostedGlassPixelationEffect.png" width="960px" height="540px" title="Frosted Glass Pixelation Effect" alt="Frosted Glass Pixelation Effect"></a> </p>
<br/>
<p align="left"> Frosted Glass Screen Wave Effect </p>
<p align="left"> <a href="https://vimeo.com/355855225"><img src="images/FrostedGlassScreenWaveEffect.png" width="960px" height="540px" title="Frosted Glass Screen Wave Effect" alt="Frosted Glass Screen Wave Effect"></a> </p>
<br/>
<p align="right"> Cross Hatching </p>
<p align="right"><a href="https://vimeo.com/355853403"><img src="images/CrossHatching.png" width="960px" height="540px" title="Cross Hatching" alt="Cross Hatching"></a> </p>
<br/>
<p align="left"> Predators Thermal Vision </p>
<p align="left"> <a href="https://vimeo.com/355856468"><img src="images/PredatorsThermalVision.png" width="960px" height="540px" title="Predators Thermal Vision" alt="Predators Thermal Vision"></a> </p>
<br/>
<p align="right"> Toonify </p>
<p align="right"><a href="https://vimeo.com/355857166"><img src="images/Toonify.png" width="960px" height="540px" title="Toonify" alt="Toonify"></a> </p>
<br/>
<p align="left"> Shockwave </p>
<p align="left"> <a href="https://vimeo.com/355857032"><img src="images/Shockwave.png" width="960px" height="540px" title="Shockwave" alt="Shockwave"></a> </p>
<br/>
<p align="right"> Fish Eye </p>
<p align="right"><a href="https://vimeo.com/355853503"><img src="images/FishEye.png" width="960px" height="540px" title="Fish Eye" alt="Fish Eye"></a> </p>
<br/>
<p align="left"> Barrel Distortion </p>
<p align="left"> <a href="https://vimeo.com/355852385"><img src="images/BarrelDistortion.png" width="960px" height="540px" title="Barrel Distortion" alt="Barrel Distortion"></a> </p>
<br/>
<p align="right"> Multi-Screen Fish Eye </p>
<p align="right"><a href="https://vimeo.com/355856355"><img src="images/MultiScreenFishEye.png" width="960px" height="540px" title="Multi-Screen Fish Eye" alt="Multi-Screen Fish Eye"></a> </p>
<br/>
<p align="left"> Fish Eye Lens </p>
<p align="left"> <a href="https://vimeo.com/355854021"><img src="images/FishEyeLens.png" width="960px" height="540px" title="Fish Eye Lens" alt="Fish Eye Lens"></a> </p>
<br/>
<p align="right"> Fish Eye / Anti-Fish Eye </p>
<p align="right"><a href="https://vimeo.com/355853810"><img src="images/FishEyeAntiFishEye.png" width="960px" height="540px" title="Fish Eye / Anti-Fish Eye" alt="Fish Eye / Anti-Fish Eye"></a> </p>
<br/>
<p align="left"> Gaussian Blur </p>
<p align="left"> <a href="https://vimeo.com/355855503"><img src="images/GaussianBlur.png" width="960px" height="540px" title="Gaussian Blur" alt="Gaussian Blur"></a> </p>
<br/>
<p align="right"> Blur </p>
<p align="right"><a href="https://vimeo.com/355852994"><img src="images/Blur.png" width="960px" height="540px" title="Blur" alt="Blur"></a> </p>
<br/>
<p align="left"> Radial Blur </p>
<p align="left"> <a href="https://vimeo.com/355856744"><img src="images/RadialBlur.png" width="960px" height="540px" title="Radial Blur" alt="Radial Blur"></a> </p>
<br/>
<p align="right"> Motion Blur </p>
<p align="right"><a href="https://vimeo.com/355856138"><img src="images/MotionBlur.png" width="960px" height="540px" title="Motion Blur" alt="Motion Blur"></a> </p>
<br/>
<p align="left"> Vignetting </p>
<p align="left"> <a href="https://vimeo.com/355857463"><img src="images/Vignetting.png" width="960px" height="540px" title="Vignetting" alt="Vignetting"></a> </p>
<br/>
<p align="right"> Bright Parts </p>
<p align="right"><a href="https://vimeo.com/355853121"><img src="images/BrightParts.png" width="960px" height="540px" title="Bright Parts" alt="Bright Parts"></a> </p>
<br/>
<p align="left"> Bloom </p>
<p align="left"> <a href="https://vimeo.com/355852729"><img src="images/Bloom.png" width="960px" height="540px" title="Bloom" alt="Bloom"></a> </p>
<br/>
<p align="right"> Lens Flare </p>
<p align="right"><a href="https://vimeo.com/355855785"><img src="images/LensFlare.png" width="960px" height="540px" title="Lens Flare" alt="Lens Flare"></a> </p>
<br/>
<p align="left"> Screen Space Ambient Occlusion </p>
<p align="left"> <a href="https://vimeo.com/356995945"><img src="images/SSAO.png" width="960px" height="540px" title="Screen Space Ambient Occlusion" alt="Screen Space Ambient Occlusion"></a> </p>
<br/>
<p align="right"> Depth Testing </p>
<p align="right"><a href="https://vimeo.com/356995712"><img src="images/DepthTesting.png" width="960px" height="540px" title="Depth Testing" alt="Depth Testing"></a> </p>
<br/>
<p align="left"> Depth Mapping </p>
<p align="left"> <a href="https://vimeo.com/356995607"><img src="images/DepthMapping.png" width="960px" height="540px" title="Depth Mapping" alt="Depth Mapping"></a> </p>
<br/>
<p align="right"> Directional Shadow Mapping </p>
<p align="right"><a href="https://vimeo.com/356988730"><img src="images/DirectionalShadowMapping.png" width="960px" height="540px" title="Directional Shadow Mapping" alt="Directional Shadow Mapping"></a> </p>
<br/>
<p align="left"> Omnidirectional Shadow Mapping </p>
<p align="left"> <a href="https://vimeo.com/356995150"><img src="images/OmnidirectionalShadowMapping.png" width="960px" height="540px" title="Omnidirectional Shadow Mapping" alt="Omnidirectional Shadow Mapping"></a> </p>
<br/>
<p align="right"> Deferred Rendering </p>
<p align="right"><a href="https://vimeo.com/356993810"><img src="images/DeferredRendering.png" width="960px" height="540px" title="Deferred Rendering" alt="Deferred Rendering"></a> </p>
<br/>
<p align="left"> Rain Drops </p>
<p align="left"> <a href="https://vimeo.com/356993509"><img src="images/RainDrops.png" width="960px" height="540px" title="Rain Drops" alt="Rain Drops"></a> </p>
<br/>
<p align="right"> Palette Quantization And Dithering </p>
<p align="right"><a href="https://vimeo.com/356993121"><img src="images/PaletteQuantizationAndDithering.png" width="960px" height="540px" title="Palette Quantization And Dithering" alt="Palette Quantization And Dithering"></a> </p>
<br/>
<p align="left"> Distorted TV </p>
<p align="left"> <a href="https://vimeo.com/356992871"><img src="images/DistortedTV.png" width="960px" height="540px" title="Distorted TV" alt="Distorted TV"></a> </p>
<br/>
<p align="right"> RGB Display </p>
<p align="right"><a href="https://vimeo.com/356991223"><img src="images/RGBDisplay.png" width="960px" height="540px" title="RGB Display" alt="RGB Display"></a> </p>
<br/>
<p align="left"> Retro Parallax </p>
<p align="left"> <a href="https://vimeo.com/356992161"><img src="images/RetroParallax.png" width="960px" height="540px" title="Retro Parallax" alt="Retro Parallax"></a> </p>
<br/>
<p align="right"> Money Filter </p>
<p align="right"><a href="https://vimeo.com/356989916"><img src="images/MoneyFilter.png" width="960px" height="540px" title="Money Filter" alt="Money Filter"></a> </p>
<br/>
<p align="left"> Microprism Mosaic </p>
<p align="left"> <a href="https://vimeo.com/356994983"><img src="images/MicroprismMosaic.png" width="960px" height="540px" title="Microprism Mosaic" alt="Microprism Mosaic"></a> </p>
<br/>
<p align="right"> Bayer Matrix Dithering </p>
<p align="right"><a href="https://vimeo.com/356991112"><img src="images/BayerMatrixDithering.png" width="960px" height="540px" title="Bayer Matrix Dithering" alt="Bayer Matrix Dithering"></a> </p>
<br/>
<p align="left"> Julia Freak </p>
<p align="left"> <a href="https://vimeo.com/356991914"><img src="images/JuliaFreak.png" width="960px" height="540px" title="Julia Freak" alt="Julia Freak"></a> </p>
<br/>
<p align="right"> Heart Blend </p>
<p align="right"><a href="https://vimeo.com/356989665"><img src="images/HeartBlend.png" width="960px" height="540px" title="Heart Blend" alt="Heart Blend"></a> </p>
<br/>
<p align="left"> EM Interference </p>
<p align="left"> <a href="https://vimeo.com/356990182"><img src="images/EMInterference.png" width="960px" height="540px" title="EM Interference" alt="EM Interference"></a> </p>
<br/>
<p align="right"> Cubic Lens Distortion </p>
<p align="right"><a href="https://vimeo.com/356990856"><img src="images/CubicLensDistortion.png" width="960px" height="540px" title="Cubic Lens Distortion" alt="Cubic Lens Distortion"></a> </p>
<br/>
<p align="left"> Cel Shaderish </p>
<p align="left"> <a href="https://vimeo.com/356991546"><img src="images/CelShaderish.png" width="960px" height="540px" title="Cel Shaderish" alt="Cel Shaderish"></a> </p>
<br/>
<p align="right"> Cartoon Video </p>
<p align="right"><a href="https://vimeo.com/356989226"><img src="images/CartoonVideo.png" width="960px" height="540px" title="Cartoon Video" alt="Cartoon Video"></a> </p>
<br/>


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
