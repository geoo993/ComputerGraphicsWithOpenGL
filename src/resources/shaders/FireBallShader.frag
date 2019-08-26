#version 400 core

//https://www.clicktorelease.com/blog/vertex-displacement-noise-3d-webgl-glsl-three-js/
//https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
//https://thebookofshaders.com/11/
//https://www.clicktorelease.com/code/perlin/chrome.html

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

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec3 vWorldTangent;
    vec4 vEyePosition;
    float vNoise;
} fs_in;


//gl_FragCoord is vec3(vTexCoord,1.0) or vTexColour.xyz
float random( vec3 scale, float seed ){
    return fract( sin( dot( gl_FragCoord.xyz + seed, scale ) ) * 43758.5453f + seed ) ;
}

layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{
    
    // get a random offset
    float r = 0.01f * random( vec3( 12.9898f, 78.233f, 151.7182f ), 0.2f );
    // lookup vertically in the texture, using noise and offset
    // to get the right RGB colour
    vec2 tPos = vec2( 0.0f, 1.3f * fs_in.vNoise + r );
    vec4 vTexColour = texture(material.displacementMap, tPos );
    //vec3 vTangentColour = fs_in.vWorldTangent * 0.5f + 0.5f;
    
    vOutputColour = vec4( vTexColour.rgb, 1.0f);
 
    // Retrieve bright parts
    float brightness = dot(vOutputColour.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f) {
        vBrightColor = vec4(vOutputColour.rgb, 1.0f);
    } else {
        vBrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // store the fragment position vector in the first gbuffer texture
    vPosition = material.bUseAO ? fs_in.vEyePosition.xyz : fs_in.vWorldPosition;
    // also store the per-fragment normals into the gbuffer
    vNormal = normalize(fs_in.vWorldNormal);
    // and the diffuse per-fragment color
    vAlbedoSpec = material.bUseAO ? vec4(0.95f, 0.95f, 0.95f, 1.0f) : texture(material.displacementMap, fs_in.vTexCoord);
}

