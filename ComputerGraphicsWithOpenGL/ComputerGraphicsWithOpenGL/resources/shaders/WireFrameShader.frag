#version 400 core

// http://codeflow.org/entries/2012/aug/02/easy-wireframe-display-with-barycentric-coordinates/
// https://forum.libcinder.org/topic/wireframe-shader-implementation
// https://pastebin.com/G9grT2Kp
// https://gamedev.stackexchange.com/questions/136915/geometry-shader-wireframe-not-rendering-correctly-glsl-opengl-c
// http://people.inf.elte.hu/plisaai/pdf/David%20Wolff%20-%20OpenGL%204.0%20Shading%20Language%20Cookbook%20(2).pdf  // note: page 211

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
    sampler2D reflectionMap;        // 10.  reflection map
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
    vec3 vWorldTangent;
    vec4 vEyePosition;
} fs_in;

uniform bool bUseTexture, bWireFrame;
uniform float thickness;

noperspective in vec3 GEdgeDistance;

out vec4 vOutputColour;		// The output colour

void main()
{
    //base fragment color off of which edge is closest
    float distance = min(GEdgeDistance[0], min(GEdgeDistance[1], GEdgeDistance[2]));
    
    if (bWireFrame) {
        
        if (distance < thickness){
            vOutputColour = bUseTexture
            ? texture(material.diffuseMap, fs_in.vTexCoord) : material.color; //draw fragment if close to edge
        }else if (distance >= thickness){
            discard; //discard if not
        }
    } else {
        //draw the each side with different color, displaying the barymetric concept
        if (distance==GEdgeDistance[0])vOutputColour=vec4(0.52f, 0.0f, 0.0f, 1.0f);
        else if (distance==GEdgeDistance[1])vOutputColour=vec4(0.0f, 0.52f, 0.0f, 1.0f);
        else if (distance==GEdgeDistance[2])vOutputColour=vec4(0.0f, 0.0f, 0.52f, 1.0f);
    }
    
}

