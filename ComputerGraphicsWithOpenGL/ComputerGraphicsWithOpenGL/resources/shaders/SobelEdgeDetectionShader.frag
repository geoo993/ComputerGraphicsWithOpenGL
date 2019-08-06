#version 400 core

// https://www.geeks3d.com/20110201/sobel-and-frei-chen-edge-detectors-demo-glsl/
// http://rastergrid.com/blog/2011/01/frei-chen-edge-detector/

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
uniform mat3 G[2] = mat3[](
                           mat3( 1.0, 2.0, 1.0,  0.0, 0.0, 0.0,   -1.0, -2.0, -1.0 ),
                           mat3( 1.0, 0.0, -1.0,  2.0, 0.0, -2.0,  1.0, 0.0, -1.0 )
                           );

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
 
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        mat3 I;
        float cnv[2];
        vec3 color = vec3(0.0);
        vec4 fragColor = vec4(gl_FragCoord.x / 2, gl_FragCoord.y / 2, gl_FragCoord.z, 1/ gl_FragCoord.w);
        // fetch the 3x3 neighbourhood and use the RGB vector's length as intensity value
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                color = texelFetch(material.ambientMap, ivec2(fragColor) + ivec2(i-1,j-1), 0 ).rgb;
                I[i][j] = length(color);
            }
        }
        
        // calculate the convolution values for all the masks
        for (int i=0; i<2; i++) {
            float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]);
            cnv[i] = dp3 * dp3;
        }
        
        tc = vec4(0.5f * sqrt(cnv[0] * cnv[0] + cnv[1] * cnv[1]));
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
