#version 400 core

//http://john-chapman-graphics.blogspot.co.uk/2013/02/pseudo-lens-flare.html

in vec2 vTexCoord;	//fbo texture	
in vec3 vWorldPosition;
in vec3 vLocalNormal;

uniform sampler2D sampler0;
uniform sampler2D lensSampler;
uniform bool bUselensTexture;
uniform int numberOfGhosts; // number of ghost samples
uniform float ghostDispersal; // dispersion factor, between 0 and 1
uniform bool useHalo;
uniform float haloWidth; // dispersion factor, between 0 and 1
uniform float distortion;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

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

void main()
{
    
    vec2 texcoord = vec2(vTexCoord.x, vTexCoord.y);
    
    int nGhosts = clamp(numberOfGhosts,2,6) ; // 2 to 6
    float dispersal = clamp(ghostDispersal,0.4f,0.6f); //0.4 to 0.6
    
    //vec2 texcoord = -vTexCoord + vec2(1.0f);
    vec2 texelSize = 1.0f / vec2(textureSize(sampler0, 0));
    vec3 distortion = vec3(-texelSize.x * distortion, 0.0f, texelSize.x * distortion);
    
    // ghost vector to image centre:
    vec2 ghostVector = (vec2(0.5f) - texcoord) * dispersal;
    
    vec2 direction = normalize(ghostVector);
    
    // sample ghosts:  
    vec4 result = vec4(0.0f);
    for (int i = 0; i < nGhosts; ++i) { 
        
        if (useHalo == true){
            // sample halo:
            vec2 offset = fract(texcoord + ghostVector );
            vec2 haloVector = normalize(ghostVector) * haloWidth;
            float weight = length( vec2(0.5f) - offset) / length( vec2(0.5f) );
            weight = pow(1.0f - weight, 5.0f);
            //result += texture(sampler0, texcoord + haloVector) * weight;
            result += textureDistorted(sampler0, texcoord + haloVector, direction, distortion) * weight;
        }else {
        
            vec2 offset = fract(texcoord + ghostVector * float(i)) ;
            float weight = length(vec2(0.5f) - (offset)) / length(vec2(0.5f));
            weight = pow(1.0f - weight, 5.0f);
            //result += texture(sampler0, offset) * weight;
            result += textureDistorted(sampler0, offset, direction, distortion) * weight;
        }
        
        
    }
    
    float lengthResult = length( vec2(0.5f) - texcoord) / length( vec2(0.5f) );
    if (bUselensTexture)
        result *= texture( lensSampler, vec2( lengthResult ) );
    
    
    vOutputColour = result;
    
}
