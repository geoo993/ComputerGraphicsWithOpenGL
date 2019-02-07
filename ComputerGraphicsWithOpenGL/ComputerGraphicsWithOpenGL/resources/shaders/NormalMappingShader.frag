#version 400 core

// http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
// https://learnopengl.com/#!Advanced-Lighting/Normal-Mapping
// http://www.thetenthplanet.de/archives/1180
// 

precision highp float;

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
    
    vec3 color;
    float shininess;
} material;

// Lights and materials passed in as uniform variables from client programme
uniform bool bUseTexture;
uniform vec2 uv_tiling;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

/*
// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
    
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
    
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}


vec3 perturb_normal( sampler2D normalSampler, vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
    vec3 map = texture(normalSampler, texcoord ).xyz;
    map = map * 255.0f/127.0f - 128.0f/127.0f;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}

*/
out vec4 vOutputColour;        // The output colour formely  gl_FragColor


void main()
{
  
    /*
         Tangent vectors (or TBN for Tangent Binormal Normal) are useful in many situations like bump/normal mapping and the TBN vectors are usually precomputed from texture coordinates and stored as a vertex attribute.
     
     
     */
    /*
    vec2 uv = vTexCoord * uv_tiling;
    vec3 lightDir = light.position - vEyePosition;
    vec3 eyeVec = vEyePosition; //-vEyePosition;
    vec3 N = normalize(vEyeNormal.xyz);
    vec3 L = normalize(lightDir.xyz);
    vec3 V = normalize(eyeVec.xyz);
    vec3 PN = perturb_normal(material.normalMap, N, V, uv);
    
    vec3 texture_color = texture(material.ambientMap, uv).rgb;
    vec3 final_color = vec3(0.2f, 0.15f, 0.15f) * texture_color;
    
    float lambertTerm = dot(PN, L);
    if (lambertTerm > 0.0f)
    {
        final_color += light.Ld * material.Md * lambertTerm * texture_color;  
        
        vec3 E = normalize(eyeVec.xyz);
        vec3 R = reflect(-L, PN);
        float specular = pow( max(dot(R, E), 0.0), material.shininess);
        final_color += light.Ls * material.Ms * specular;  
    }
    vOutputColour.rgb = bUseTexture ? final_color : PN.rgb;
    vOutputColour.a = 1.0f;
     */
    
    vec4 vTexColour0 = texture(material.ambientMap, fs_in.vTexCoord);
    vec4 vTexColour1 = texture(material.normalMap, fs_in.vTexCoord);
    vec4 vTexColour2 = texture(material.diffuseMap, fs_in.vTexCoord);
    vec4 vTexColour3 = texture(material.specularMap, fs_in.vTexCoord);
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 worldPos = fs_in.vWorldPosition;
    //vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    vOutputColour = vTexColour3; //vec4(normal, 1.0f);
}
