#version 400 core

// http://glampert.com/2014/01-26/visualizing-the-depth-buffer/
// https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
// https://stackoverflow.com/questions/26406120/viewing-depth-buffer-in-opengl
// https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping

// Structure holding material information:  its ambient, diffuse, specular, etc...
uniform struct Material
{
    sampler2D ambientMap;           // 0.   ambient map (albedo map)
    sampler2D diffuseMap;           // 1.   diffuse map (metallic map)
    sampler2D specularMap;          // 2.   specular map (roughness map)
    sampler2D normalMap;            // 3.   normal map
    sampler2D heightMap;            // 4.   height map
    sampler2D emissionMap;          // 5.   emission map
    sampler2D displacementMap;      // 6.   displacment map
    sampler2D aoMap;                // 7.   ambient oclusion map
    sampler2D glossinessMap;        // 8.   glossiness map
    sampler2D opacityMap;           // 9.   opacity map
    samplerCube shadowMap;          // 10.  shadow cube map
    sampler2D depthMap;             // 11.  depth map
    sampler2D noiseMap;             // 12.  noise map
    sampler2D maskMap;              // 13.  mask map
    sampler2D lensMap;              // 14.  lens map
    samplerCube cubeMap;            // 15.  sky box or environment mapping cube map
    vec4 color;
    float shininess;
    float uvTiling;
} material;

uniform struct Camera
{
    vec3 position;
    vec3 front;
    float znear;
    float zfar;
    bool isMoving;
    bool isOrthographic;
} camera;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

// required when using a perspective projection matrix
float LinearizeDepth(float depth, float near, float far)
{
    float z = depth * 2.0f - 1.0f; // Back to NDC 
    return (2.0f * near * far) / (far + near - z * (far - near));
}

out vec4 vOutputColour;    

void main()
{

    float depthValue = gl_FragCoord.z;
    float depth = LinearizeDepth(depthValue, camera.znear, camera.zfar) / camera.zfar;
    vOutputColour = vec4(vec3(depth), 1.0f);
}

