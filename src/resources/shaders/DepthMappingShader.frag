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
    vec4 guiColor;
    float shininess;
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

uniform struct Shadow
{
    float znear;
    float zfar;
    float bias;
    bool bFromLightOrCamera;
    bool bShowDepth;
} shadow;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

uniform float coverage;        // between (0.0f and 1.0f)

// required when using a perspective projection matrix
float LinearizeDepth(float depth, float near, float far)
{
    float z = depth * 2.0f - 1.0f; // Back to NDC
    return (2.0f * near * far) / (far + near - z * (far - near));
}

out vec4 vOutputColour;    

void main()
{
    /*
     Sometimes it can be quite useful to visualize the depth buffer of a rendered frame.
     Several rendering techniques such as shadow mapping and depth pre-pass rely on the depth buffer.
     It is always handy to visualize it in real time to make sure it is being written as expected.
     
     */
   
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        float depthValue = texture(material.depthMap, uv).r; // orthographic
        float depth = LinearizeDepth(depthValue, shadow.znear, shadow.zfar) / shadow.zfar; // divide by zfar for perspective
        tc = vec4(vec3(camera.isOrthographic ? depthValue : depth), 1.0f);
    }
    else if ( uv.x  >=  (  coverage  +   0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    }
    else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
}

