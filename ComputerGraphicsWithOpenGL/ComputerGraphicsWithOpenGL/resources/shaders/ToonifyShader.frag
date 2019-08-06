#version 400 core

// http://www.geeks3d.com/20140523/glsl-shader-library-toonify-post-processing-filter/

in vec2 vTexCoord;
in vec3 vWorldPosition;
in vec3 vLocalNormal;

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

uniform float coverage;        // between (0.0f and 1.0f)
uniform float lower_edge_thres = 0.2f; // must be lower than upper_edge_thres
uniform float upper_edge_thres = 5.0f; // must be higher than lower_edge_thres


#define HueLevCount 6
#define SatLevCount 7
#define ValLevCount 4

float[HueLevCount] HueLevels = float[] (0.0f, 140.0f, 160.0f, 240.0f, 240.0f, 360.0f);
float[SatLevCount] SatLevels = float[] (0.0f, 0.15f, 0.3f, 0.45f, 0.6f, 0.8f, 1.0f);
float[ValLevCount] ValLevels = float[] (0.0f, 0.3f, 0.6f, 1.0f);

vec3 RGBtoHSV( float r, float g, float b)
{
    float minv, maxv, delta;
    vec3 res;
    
    minv = min( min(r, g), b);
    maxv = max( max(r, g), b);
    res.z = maxv;            // v
    
    delta = maxv - minv;
    
    if( maxv != 0.0f )
        res.y = delta / maxv;      // s
    else {
        // r = g = b = 0      // s = 0, v is undefined
        res.y = 0.0f;
        res.x = -1.0f;
        return res;
    }
    
    if( r == maxv )
        res.x = ( g - b ) / delta;      // between yellow & magenta
    else if( g == maxv )
        res.x = 2.0f + ( b - r ) / delta;   // between cyan & yellow
    else
        res.x = 4.0f + ( r - g ) / delta;   // between magenta & cyan
    
    res.x = res.x * 60.0f;            // degrees
    if( res.x < 0.0f )
        res.x = res.x + 360.0f;
    
    return res;
}

vec3 HSVtoRGB(float h, float s, float v )
{
    int i;
    float f, p, q, t;
    vec3 res;
    
    if( s == 0.0f )
    {
        // achromatic (grey)
        res.x = v;
        res.y = v;
        res.z = v;
        return res;
    }
    
    h /= 60.0f;         // sector 0 to 5
    i = int(floor( h ));
    f = h - float(i);         // factorial part of h
    p = v * ( 1.0f - s );
    q = v * ( 1.0f - s * f );
    t = v * ( 1.0f - s * ( 1.0f - f ) );
    
    switch(i)
    {
        case 0:
            res.x = v;
            res.y = t;
            res.z = p;
            break;
        case 1:
            res.x = q;
            res.y = v;
            res.z = p;
            break;
        case 2:
            res.x = p;
            res.y = v;
            res.z = t;
            break;
        case 3:
            res.x = p;
            res.y = q;
            res.z = v;
            break;
        case 4:
            res.x = t;
            res.y = p;
            res.z = v;
            break;
        default:      // case 5:
            res.x = v;
            res.y = p;
            res.z = q;
            break;
    }
    return res;
}

float nearestLevel(float col, int mode)
{
    int levCount;
    if (mode == 0) levCount = HueLevCount;
    if (mode == 1) levCount = SatLevCount;
    if (mode == 2) levCount = ValLevCount;
    
    for (int i = 0; i < levCount-1; i++ ) {
        if ( mode == 0) {
            if (col >= HueLevels[i] && col <= HueLevels[i+1]) {
                return HueLevels[i+1];
            }
        }
        if ( mode == 1) {
            if (col >= SatLevels[i] && col <= SatLevels[i+1]) {
                return SatLevels[i+1];
            }
        }
        if ( mode == 2 ) {
            if (col >= ValLevels[i] && col <= ValLevels[i+1]) {
                return ValLevels[i+1];
            }
        }
    }
}

// averaged pixel intensity from 3 color channels
float avg_intensity(vec4 pix)
{
    return (pix.r + pix.g + pix.b) / 3.0f;
}

vec4 get_pixel(sampler2D sampler, vec2 coords, float dx, float dy)
{
    return texture(sampler, coords + vec2(dx, dy));
}

// returns pixel color
float IsEdge(in sampler2D sampler, in vec2 coords, in float edgeThreshold)
{
    float dxtex = 1.0 / float( textureSize(sampler, 0) ) ;
    float dytex = 1.0 / float( textureSize(sampler, 0 ) );
    float pix[9];
    int k = -1;
    float delta;
    
    // read neighboring pixel intensities
    for (int i=-1; i<2; i++) {
        for(int j=-1; j<2; j++) {
            k++;
            pix[k] = avg_intensity( get_pixel(sampler, coords, float(i) * dxtex, float(j) * dytex) );
        }
    }
    
    // average color differences around neighboring pixels
    delta = (abs( pix[1] - pix[7]) +
             abs( pix[5] - pix[3]) +
             abs( pix[0] - pix[8]) +
             abs( pix[2] - pix[6])
             ) / 4.0f;
    
    //return clamp(5.5*delta,0.0,1.0);
    return clamp( edgeThreshold * delta, 0.0f, 1.0f);
}

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
  
    /*   Toonify Shader
     
         Here is a new entry in the GLSL Shader Library: a toonify post processing filter. This filter creates a cartoon version of an image. 
         You can use the mouse (the vertical red line) to see the difference between the original and toonified versions of the image.
     
     */
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        vec3 colorOrg = texture(material.ambientMap, uv).rgb;
        vec3 vHSV =  RGBtoHSV( colorOrg.r, colorOrg.g, colorOrg.b);
        vHSV.x = nearestLevel(vHSV.x, 0);
        vHSV.y = nearestLevel(vHSV.y, 1);
        vHSV.z = nearestLevel(vHSV.z, 2);
        float edg = IsEdge(material.ambientMap, uv, upper_edge_thres);
        vec3 vRGB = (edg >= lower_edge_thres) ? vec3(0.0f, 0.0, 0.0f) : HSVtoRGB(vHSV.x, vHSV.y, vHSV.z);
        tc = vec4(vRGB.x, vRGB.y, vRGB.z, 1.0f);
   
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
