# Author: George Quentin
# Date: 10th August 2019
# Computer Graphics project with OpenGL

# set cmake minimum version
cmake_minimum_required (VERSION 3.0)


# project name
project (ComputerGraphicsWithOpenGL)

# set config properties
set (ComputerGraphicsWithOpenGL_VERSION_MAJOR 1)
set (ComputerGraphicsWithOpenGL_VERSION_MINOR 0)
set (ComputerGraphicsWithOpenGL_RESOURCES "${PROJECT_SOURCE_DIR}/src")


# create config header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ComputerGraphicsWithOpenGLConfig.h.in"
  "${PROJECT_BINARY_DIR}/ComputerGraphicsWithOpenGLConfig.h"
)


# set Xcode build settings properties
set (XCODE_ATTRIBUTE_SDKROOT macosx)
set (CMAKE_OSX_SYSROOT macosx)
set (CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
set (CMAKE_OSX_ARCHITECTURES ${ARCHS_STANDARD_32_64_BIT})
set (CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14") # can use c++0x as default
set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")


# add a subdirectory to the project.
add_subdirectory( src )