#version 410 core

// http://glampert.com/2014/01-26/visualizing-the-depth-buffer/
// https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
// https://stackoverflow.com/questions/26406120/viewing-depth-buffer-in-opengl
// https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping

#define NUMBER_OF_POINT_LIGHTS 10

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
    bool bFromLightOrCamera;
} shadow;

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
    bool bUseAO;
    bool bUseTexture;
    bool bUseColor;
} material;

uniform vec3 lightPos;


in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // ise the fragment to light vector to sample from the depth map
 
    float closestDepth = texture(material.cubeMap, fragToLight).r;
    
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= shadow.zfar;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float bias = 0.05f; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth -  bias > closestDepth ? 1.0f : 0.0f;
    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);
    
    
    return shadow;
}

//When rendering into the current framebuffer, whenever a fragment shader uses the layout location specifier the respective colorbuffer of framebuffor colors array, which is used to render the fragments to that color buffer.
layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{
    vec3 color = texture(material.diffuseMap, fs_in.vTexCoord).rgb;
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 lightColor = vec3(0.3f);
    vec3 worldPos = fs_in.vWorldPosition;
    vec3 viewPos = camera.position;// + camera.front;
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
     
    // ambient
    vec3 ambient = 0.3f * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0f;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
    vec3 specular = spec * lightColor;
    // calculate shadow
    float shadow = ShadowCalculation(worldPos);
    vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;
    
    vOutputColour = vec4(lighting, 1.0f);
    
//    // get vector between fragment position and light position
//    vec3 fragToLight = worldPos - lightPos;
//    // ise the fragment to light vector to sample from the depth map
//
//    float closestDepth = texture(material.cubeMap, fragToLight).r;
//    vOutputColour = vec4(vec3(closestDepth / shadow.zfar), 1.0f);
}

