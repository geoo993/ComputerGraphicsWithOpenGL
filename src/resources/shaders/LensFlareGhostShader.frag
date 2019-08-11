#version 400 core

// http://john-chapman-graphics.blogspot.co.uk/2013/02/pseudo-lens-flare.html
// https://john-chapman.github.io/2017/11/05/pseudo-lens-flare.html
// https://www.youtube.com/watch?v=OiMRdkhvwqg
// https://www.youtube.com/watch?v=AjSr0zLBnx8
// https://github.com/john-chapman/GfxSamples

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

uniform int lensType;           // between 1 and 3
uniform bool bUselensTexture;
uniform int ghostCount = 5;         // number of ghost samples, between 4 and 32
uniform float ghostDispersal = 0.39f;   // dispersion factor, between 0 and 2
uniform float ghostThreshold = 10.0f;   // between 0.0f and 20.0f
uniform float ghostDistortion = 4.3f;
uniform float haloRadius = 0.3f;       // between 0.0f and 2.0f
uniform float haloThreshold = 9.0f;    // between 0.0f and 20.0f

// This can be used as a direct replacement for the calls to texture()
vec4 textureDistorted(
                      in sampler2D tex,
                      in vec2 texcoord,
                      in vec2 direction, // direction of distortion
                      in vec3 distortion // per-channel distortion factor
                      ) {
    
    return vec4(
                texture(tex, texcoord + direction * distortion.r).r,
                texture(tex, texcoord + direction * distortion.g).g,
                texture(tex, texcoord + direction * distortion.b).b,
                1.0f
                );
}

vec4 applyHalo(
               in sampler2D tex,
               in vec2 texcoord,
               in vec2 ghostVec,
               in vec2 direction, // direction of distortion
               in vec3 distortion // per-channel distortion factor
) {
    // sample halo:
    
    // We can use this to produce a "halo", weighting the sample to to restrict the contribution of
    // the warped image to a ring, the radius of which is controlled by haloRadius:
    vec2 haloVec = normalize(ghostVec) * haloRadius;
    vec2 offset = texcoord + haloVec;
    float weight = length(vec2(0.5f) - fract(offset)) / length(vec2(0.5f));
    weight = pow(1.0f - weight, haloThreshold);
    
    //result += texture(tex, offset) * weight;
    //// CHROMATIC DISTORTION
    return textureDistorted(tex, offset, direction, distortion) * weight;
}

vec4 applyGhost(
                in sampler2D tex,
                in vec2 texcoord,
                in int index,
                in vec2 ghostVec,
                in vec2 direction, // direction of distortion
                in vec3 distortion // per-channel distortion factor
) {
    // sample ghosts:
    
    vec2 offset = fract(texcoord + ghostVec * float(index));
    
    // We can improve this by allowing only bright spots from the centre of the source image to generate ghosts.
    // We do this by weighting samples by a falloff from the image centre:
    float weight = length(vec2(0.5f) - offset) / length(vec2(0.5f));
    weight = pow(1.0f - weight, ghostThreshold);
    
    // The reason we perform the weighting inside the sampling loop is so that bright spots in the centre of the input image can 'cast' ghosts to the edges,
    // but bright spots at the edges can't cast ghosts to the centre.
    //return texture(tex, offset) * weight;
    
    //// CHROMATIC DISTORTION
    return textureDistorted(tex, offset, direction, distortion) * weight;
}

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec2 texcoord = vec2(1.0f) - uv;    // flip the texture coordinates
   
    vec2 texelSize = 1.0f / vec2(textureSize(material.depthMap, 0));
    
    vec3 distortion = vec3(-texelSize.x * ghostDistortion, 0.0f, texelSize.x * ghostDistortion);
    
    // ghost vector to image centre:
    vec2 ghostVec = (vec2(0.5f) - texcoord) * ghostDispersal;
    
    vec2 direction = normalize(ghostVec);
    
    vec4 result = vec4(0.0f);
    for (int i = 0; i < ghostCount; ++i) {
        if (lensType == 0){ // Ghost
            result += applyGhost(material.depthMap, texcoord, i, ghostVec, direction, distortion);
        } else if (lensType == 1) { // Halo
            result += applyHalo(material.depthMap, texcoord, ghostVec, direction, distortion);
        } else if (lensType == 2) { // Combine Ghost and Halo
            result += applyGhost(material.depthMap, texcoord, i, ghostVec, direction, distortion);
            result += applyHalo(material.depthMap, texcoord, ghostVec, direction, distortion);
        } else {
            result += texture(material.depthMap, texcoord);
        }
    }
    
    if (bUselensTexture) {
        float lengthResult = length( vec2(0.5f) - texcoord) / length( vec2(0.5f) );
        result *= texture( (lensType == 1) ? material.lensMap : material.maskMap, vec2( lengthResult ) );
    }
       
    vOutputColour = result;
}
