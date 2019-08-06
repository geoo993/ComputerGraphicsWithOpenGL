#version 400 core

// https://www.shadertoy.com/view/XlsXDN
// Money filter by Giacomo Preciado
// Based on: "Free Engraved Illustration Effect Action for Photoshop" - http://snip.ly/j0gq
// e-mail: giacomo@kyrie.pe
// website: http://kyrie.pe

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

uniform int mouseDown;
uniform vec2 mouse;
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        // https://gamedev.stackexchange.com/questions/106674/shadertoy-getting-help-moving-to-glsl
        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml
        // https://stackoverflow.com/questions/26965787/how-to-get-accurate-fragment-screen-position-like-gl-fragcood-in-vertex-shader
        vec2 fragCoord = gl_FragCoord.xy;
        vec2 fragCoordViewportCoordinates = fragCoord * 0.5f + 0.5f;
        vec2 resolution = vec2(width, height);// width and height of the screen
        
        vec2 xy = fragCoordViewportCoordinates.xy / resolution.yy;
        
        float mouzeZ = mouseDown;
        float amplitud = 0.03f;
        float frecuencia = 10.0f;
        float gris = 1.0f;
        float divisor = 4.8f / resolution.y;
        float grosorInicial = divisor * 0.2f;
        
        const int kNumPatrones = 6;
        
        // x: seno del angulo, y: coseno del angulo, z: factor de suavizado
        vec3 datosPatron[kNumPatrones];
        datosPatron[0] = vec3(-0.7071, 0.7071, 3.0); // -45
        datosPatron[1] = vec3(0.0, 1.0, 0.6); // 0
        datosPatron[2] = vec3(0.0, 1.0, 0.5); // 0
        datosPatron[3] = vec3(1.0, 0.0, 0.4); // 90
        datosPatron[4] = vec3(1.0, 0.0, 0.3); // 90
        datosPatron[5] = vec3(0.0, 1.0, 0.2); // 0
        
        vec4 color = texture(material.ambientMap, vec2(fragCoordViewportCoordinates.x / resolution.x, xy.y));
        tc = color;
        
        for(int i = 0; i < kNumPatrones; i++)
        {
            float coseno = datosPatron[i].x;
            float seno = datosPatron[i].y;
            
            // RotaciÃ³n del patrÃ³n
            vec2 punto = vec2(
                              xy.x * coseno - xy.y * seno,
                              xy.x * seno + xy.y * coseno
                              );
            
            float grosor = grosorInicial * float(i + 1);
            float dist = mod(punto.y + grosor * 0.5f - sin(punto.x * frecuencia) * amplitud, divisor);
            float brillo = 0.3f * color.r + 0.4f * color.g + 0.3f * color.b;
            
            if(dist < grosor && brillo < 0.75f - 0.12f * float(i))
            {
                // Suavizado
                float k = datosPatron[i].z;
                float x = (grosor - dist) / grosor;
                float fx = abs((x - 0.5f) / k) - (0.5f - k) / k;
                gris = min(fx, gris);
            }
        }
        
        tc = vec4(gris, gris, gris, 1.0f);
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
