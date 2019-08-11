#version 400 core
// https://www.shadertoy.com/view/4lSGRw

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

uniform float time = 0.2f;
uniform float coverage;        // between (0.0f and 1.0f)

vec2 computeUV( vec2 uv, float k, float kcube ){
    
    vec2 t = uv - 0.5f;
    float r2 = t.x * t.x + t.y * t.y;
    float f = 0.0f;
    
    if( kcube == 0.0f){
        f = 1.0f + r2 * k;
    }else{
        f = 1.0f + r2 * ( k + kcube * sqrt( r2 ) );
    }
    
    vec2 nUv = f * t + 0.5f;
    
    return nUv;
    
}

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        float k = 1.0f * sin( time * 0.9f );
        float kcube = 0.5f * sin( time );
        
        float offset = 0.1f * sin( time * 0.5f );
        
        float red = texture( material.ambientMap, computeUV( uv, k + offset, kcube ) ).r;
        float green = texture( material.ambientMap, computeUV( uv, k, kcube ) ).g;
        float blue = texture( material.ambientMap, computeUV( uv, k - offset, kcube ) ).b;
        
        tc = vec4( red, green, blue, 1.0f );
       
    } else if (uv.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    } else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
}
