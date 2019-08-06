#version 400 core

// http://john-chapman-graphics.blogspot.co.uk/2013/01/what-is-motion-blur-motion-pictures-are.html
// http://john-chapman-graphics.blogspot.com/2013/01/per-object-motion-blur.html
// https://mynameismjp.wordpress.com/2009/03/10/reconstructing-position-from-depth/
// https://www.gamedev.net/forums/topic/619461-screen-space-motion-blur-artifacts/
// https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
// https://stackoverflow.com/questions/26406120/viewing-depth-buffer-in-opengl
// https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch27.html

uniform struct Camera
{
    vec3 position;
    vec3 front;
    float znear;
    float zfar;
    bool isMoving;
    bool isOrthographic;
} camera;

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

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

uniform int nSamples = 11;
uniform mat4 inverseModelView;  // the inverse of the current model-view matrix.
uniform mat4 inverseProjection;  // the inverse of the current projection matrix.
uniform mat4 prevMVP;           //each frame, store the camera's model-view-projection matrix so that in the next frame we'll have access to it.
uniform float currentFps;       // current frame rate
uniform float targetFps = 60.0f;        // desired frame rate
uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

// Function for converting depth to view-space position
// in deferred pixel shader pass.  vTexCoord is a texture
// coordinate for a full-screen quad, such that x=0 is the
// left of the screen, and y=0 is the top of the screen.
vec3 positionFromDepth(in vec2 texCoord)
{
    
    // Get the depth value for this pixel
    float z = texture(material.depthMap, texCoord).r;
    
    // Get x/w and y/w from the viewport position
    float x = texCoord.x * 2.0f - 1.0f;
    float y = (1.0f - texCoord.y) * 2.0f - 1.0f;
    vec4 projectedPos = vec4(x, y, z, 1.0f);
    // Transform by the inverse projection matrix
    vec4 positionVS = projectedPos * inverseProjection;
    // Divide by w to get the view-space position
    return positionVS.xyz / positionVS.w;
}

void main()
{

    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        vec2 texCoord = uv;
        mat4 invMVP = inverseProjection * inverseModelView;
        mat4 currentToPrevious = prevMVP * invMVP;
        vec4 result = texture(material.ambientMap, texCoord);
        
        // Unproject the point to 3D using the current modelview and projection matrix
        float p_depth = texture(material.depthMap, texCoord).r;
        vec2 p_ndc = texCoord.xy * 2.0f - 1.0f;
        vec4 current = invMVP * vec4(p_ndc, p_depth, 1.0f);
        current = current / current.w;
        
        // Project the point using the previous frame's modelview and projection matrix
        vec4 previous = prevMVP * current;
        previous.xyz /= previous.w; // normalised device coordinates
        previous.xy = previous.xy * 0.5f + vec2(0.5f); // texture coordinates
        
        
        /*
        // get current world space position:
        vec3 current = positionFromDepth(texCoord);
    
        vec4 previous = currentToPrevious * vec4(current, 1.0f);
        previous.xyz /= previous.w;
        previous.xy = previous.xy * 0.5f + vec2(0.5f);
        */
        
        vec2 motion = (previous.xy - texCoord);
        
        if (camera.isMoving) {
            float blurScale = currentFps / targetFps;
            
            // Construct the blur vector (scaled for extra blur)
            vec2 blurVec = motion * blurScale;
            // perform blur:
            if (previous.x != texCoord.x) {
                for (int i = 1; i < nSamples; i++) {
                    // get offset in range [-0.5, 0.5]:
                    vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5f);
                    
                    // sample & add to result:
                    result += texture(material.ambientMap, uv + offset);
                }
                result /= float(nSamples);
            }
        }
        tc = result;
        
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

