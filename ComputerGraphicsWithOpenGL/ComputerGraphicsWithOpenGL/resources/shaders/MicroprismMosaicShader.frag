#version 400 core

// https://www.shadertoy.com/view/4tj3DD
// https://hackernoon.com/converting-shaders-from-shadertoy-to-threejs-fe17480ed5c6

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
uniform float time;
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

const float minCells = 1.0f;
const float maxCells = 50.0f;
const float defaultCells = 12.0f;
const float aberration = 1.05f;

float triAspect = 0.866f; //sqrt(3.)/ 2.;

float hitTest(vec2 uv, vec2 cellSize) {
    
    vec2 cellOffset = mod(uv,cellSize) / cellSize;
    vec2 cellIdx = floor(uv/cellSize);
    vec2 subCellIdx = vec2(cellIdx.x*2.0f + step(0.5f,cellOffset.x), cellIdx.y);
    
    float oddRow = sign(mod(subCellIdx.y,2.0f));
    float oddSubCol = sign(mod(subCellIdx.x,2.0f));
    
    float testTopLeft = 1.-mod(oddRow + oddSubCol,2.0f);
    
    float tlScore = (mod(cellOffset.x,0.5f)*2.0f)-(1.0f-cellOffset.y);
    float trScore = cellOffset.y-(mod(cellOffset.x,0.5f)*2.0f);
    float tbScore = (fract(cellOffset.y+0.5f)-0.5f)*2.0f;
    
    float oddEven = sign(mix(tlScore,trScore,testTopLeft));
    
    // +/- for odd/even triangles, magnitude is distance from nearest edge
    return oddEven * min(abs(mix(tlScore,trScore,testTopLeft)),abs(tbScore));
}

float distFromTriCentre(vec2 uv, vec2 cellSize, float hitScore) {
    
    vec2 cellIdx = floor(uv/cellSize);
    float oddRow = sign(mod(cellIdx.y,2.0f));
    float oddTri = 1.-mod(oddRow + hitScore,2.0f);
    
    vec2 cellOffset = mod(uv,cellSize)/cellSize;
    cellOffset.x = fract(cellOffset.x +0.5f * sign(1.0f-oddTri));
    
    vec2 triOffset = vec2((cellOffset.x-0.5f),
                          mix(cellOffset.y-(1.0f/3.0f),
                              cellOffset.y-(1.0f-(1.0f/3.0f)),
                              hitScore));
    
    return length(triOffset)*3.0f/2.0f;
}

vec2 fixUV(vec2 uv) {
    
    // flip y unless video
    float iChannelTime = 1.0f;
    return vec2(uv.x,mix(uv.y,uv.y,sign(iChannelTime)));
}

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
    vec4 tc = material.guiColor;
    
    if (fs_in.vTexCoord.x <  coverage )
    {
        vec2 resolution = vec2(width, height);// width and height of the screen
        
        float screenAspect = resolution.x / resolution.y;
        
        float mouseZ = mouseDown;
        // set offset via mouse-x if button down, else timer
        vec2 offset = mix(vec2(0.4f*sin(time/1.5f),0.0f),
                          vec2(0.5f-(mouse.x / resolution.x),0.0f),
                          step(1.0f, mouseZ));
        offset *= mat2(0.9659f,-0.2588f,0.2588f,0.9659f); // 15deg rotation
        
        
        // set mosaic density via mouse-y if button down, else default
        float cellsHigh = mix(defaultCells,
                              mix(minCells,maxCells,1.0f-sqrt(1.0f-(mouse.y / resolution.y))),
                              step(1.0f,mouseZ));
        
        float cellsWide = cellsHigh * screenAspect * triAspect;
        vec2 cellSize = vec2(1.0f/cellsWide,1.0f/cellsHigh);
        
        vec2 uv = fixUV(fs_in.vTexCoord);
        
        // find whether co-ord in 'odd' or 'even' cell
        float score = hitTest(uv-0.5f,cellSize);
        float oddEven = step(0.0f,score);
        
        // texture lookup with chroma spread
        vec2 uvTranslate = 0.5f * cellSize * mix(-offset/4.0f,offset,oddEven);
        float chromAbr = pow(aberration,sqrt(1.2f*cellsHigh));
        vec4 txColor = vec4(
                            texture(material.ambientMap, clamp(uv+(uvTranslate/chromAbr),0.0f,1.0f)).x,
                            texture(material.ambientMap, clamp(uv+uvTranslate,0.0f,1.0f)).y,
                            texture(material.ambientMap, clamp(uv+(chromAbr*uvTranslate),0.0f,1.0f)).z,
                            1.0f);
        
        // vary brightness based on offset, distance from top of cell
        float bright = (0.04f + (0.04f * length(offset)/0.5f)) * (1.0f-(0.7f*fract((uv.y-0.5f)/cellSize.y)));
        tc = mix(txColor, mix(pow(txColor,vec4(2.0f)), vec4(1.0f)-pow(vec4(1.)-txColor,vec4(2.0f)),oddEven), vec4(bright));
        
        // vignetting based on distance from centre of cell, attenuation by cell count
        float attn = pow(0.97f,pow(cellsHigh,1.3f));
        float vignette = distFromTriCentre(uv-0.5f, cellSize, oddEven);
        tc -= 0.25f*attn * (1.0f - (pow(0.92f,3.0f*pow(vignette,2.5f))));
        
        // darken near tri edges
        float edges = 1.0f-pow(abs(score),0.5f);
        tc -= 0.5f*attn * (1.0f - (pow(0.9f,1.0f*pow(edges,4.0f))));
    } else if (fs_in.vTexCoord.x >= ( coverage + 0.003f) ) {
        tc = texture(material.ambientMap, fs_in.vTexCoord.xy);
    } else {
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, fs_in.vTexCoord.xy);
        }
    }
    
    vOutputColour = tc;
}
