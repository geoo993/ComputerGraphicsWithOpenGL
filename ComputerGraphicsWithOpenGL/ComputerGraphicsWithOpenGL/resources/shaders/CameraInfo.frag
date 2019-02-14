
/// https://stackoverflow.com/questions/9474844/using-vert-and-frag-extensions-in-apples-opengl-shader-builder

// Structure holding camera information:  its position, front vector etc...
uniform struct Camera
{
    vec3 position;
    vec3 front;
} camera;
