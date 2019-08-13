# Author: George Quentin
# Date: 10th August 2019
# Computer Graphics project with OpenGL

# set the compilation flag to report all warnings and enable debugging in the generated 
# object files and executable. we simply tell CMake which compiler and compiler version 
# you wish to build your project with. 
# If you don’t set anything, it will pick the best fit on its own.
set( CMAKE_C_FLAGS "-Wall -g" )

# the following file GLOB command will search for all files within the current directory
# that match the specified expressions (*.c and *.h), and assign the resulting files
# to the variable SRCS. Thus, SRCS is a list of all C source and headers files in the 
# current directory
file( GLOB_RECURSE SRCS *.cpp *.h)

# add the path of the include directories that you want the compiler to look 
# into while searching for header files while compiling your code. 
# This will also include the header files from 3rd party libraries as well.
include_directories(
	${PROJECT_SOURCE_DIR}/Includes
	${PROJECT_SOURCE_DIR}/Includes/freetype2
)

# add the executable target
add_executable( ComputerGraphicsWithOpenGL ${SRCS} )


# add link to libraries aka linking in compilation. 
# after including header files we now you need to tell the compiler where 
# exactly the libraries of the header files are located.
link_directories( ${PROJECT_SOURCE_DIR}/Libraries )


# add all system frameworks from using the linking paths.
macro(ADD_SYSTEM_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS /System/Library
        PATH_SUFFIXES Frameworks
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} ${FRAMEWORK_${fwname}})
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_SYSTEM_FRAMEWORK)

ADD_SYSTEM_FRAMEWORK(OpenGL ComputerGraphicsWithOpenGL)
ADD_SYSTEM_FRAMEWORK(Cocoa ComputerGraphicsWithOpenGL)
# target_link_libraries(ComputerGraphicsWithOpenGL 
# 			"-framework Cocoa"
# 			"-framework OpenGL")

# add all external libraries/frameworks using the linking paths.
macro(ADD_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${PROJECT_SOURCE_DIR}/Libraries
        PATH_SUFFIXES Libraries
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} ${FRAMEWORK_${fwname}})
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_FRAMEWORK)

ADD_FRAMEWORK(libassimp.4.1.0.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libfmod.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libfmodL.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libfmodstudio.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libfmodstudioL.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libfreeimage.3.17.0.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libfreeimageplus.3.17.0.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libfreetype.6.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libpng16.16.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libGLEW.2.1.0.dylib ComputerGraphicsWithOpenGL)
ADD_FRAMEWORK(libglfw.3.2.dylib ComputerGraphicsWithOpenGL)


