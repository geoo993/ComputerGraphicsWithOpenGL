#version 400 core
// https://www.shadertoy.com/view/4dX3DM

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

uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        // https://gamedev.stackexchange.com/questions/106674/shadertoy-getting-help-moving-to-glsl
        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml
        // https://stackoverflow.com/questions/26965787/how-to-get-accurate-fragment-screen-position-like-gl-fragcood-in-vertex-shader
        vec2 fragCoord = gl_FragCoord.xy;
        vec2 fragCoordViewportCoordinates = fragCoord * 0.5f + 0.5f;
        vec2 resolution = vec2(width, height);// width and height of the screen
        
        int CELL_SIZE = 6;
        float CELL_SIZE_FLOAT = float(CELL_SIZE);
        int RED_COLUMNS = int(CELL_SIZE_FLOAT/3.0f);
        int GREEN_COLUMNS = CELL_SIZE-RED_COLUMNS;
        
        vec2 p = floor(fragCoordViewportCoordinates.xy / CELL_SIZE_FLOAT)*CELL_SIZE_FLOAT;
        int offsetx = int(mod(fragCoordViewportCoordinates.x, CELL_SIZE_FLOAT));
        int offsety = int(mod(fragCoordViewportCoordinates.y, CELL_SIZE_FLOAT));
        
        vec4 sum = texture(material.ambientMap, p / resolution.xy);
        
        tc = vec4(0.0f,0.0f,0.0f,1.0f);
        if (offsety < CELL_SIZE-1) {
            if (offsetx < RED_COLUMNS) tc.r = sum.r;
            else if (offsetx < GREEN_COLUMNS) tc.g = sum.g;
            else tc.b = sum.b;
        }
        
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
