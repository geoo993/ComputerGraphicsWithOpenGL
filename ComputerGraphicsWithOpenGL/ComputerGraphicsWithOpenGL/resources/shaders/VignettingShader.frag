#version 400 core

in vec2 vTexCoord; //fbo texture
in vec3 vWorldPosition;
in vec3 vLocalNormal;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

uniform sampler2D sampler0;
uniform bool tint;
uniform bool useGray;
uniform bool useSepia;

//RADIUS of our vignette, where 0.5 results in a circle fitting the screen
const float RADIUS = 0.3f; // [0 - 0.5]

//softness of our vignette, between 0.0 and 1.0 or between 0.0 and 0.5
const float SOFTNESS = 0.25f; //[]

//sepia colour, adjust to taste
const vec3 SEPIA = vec3(1.2f, 1.0f, 0.8f); 

void main()
{
    
    vec4 texColour = texture(sampler0, vTexCoord);
    
    vec2 vectorFromCentrePosition = vTexCoord.xy - vec2(0.5f);
    
    //determine the vector length of the center position
    float len = length(vectorFromCentrePosition);
    
    //use smoothstep to create a smooth vignette
    float vignette = 1.0f - smoothstep(RADIUS, 1.0f - SOFTNESS, len);
    
    
    if (tint == true) {
        ////
        //apply the vignette with 50% opacity
        texColour.rgb = mix(texColour.rgb, texColour.rgb * vignette, 0.5f);
        
        
        vec3 finalColor;
        
        //2. GRAYSCALE
        
        //convert to grayscale using NTSC conversion weights
        float gray = dot(texColour.rgb, vec3(0.299f, 0.587f, 0.114f));
        vec3 grayColor = vec3(gray);
        
        if (useGray == true){
            finalColor = grayColor;
        }
        
        if (useSepia == true) {
            //3. SEPIA
            
            //create our sepia tone from some constant value
            vec3 sepiaColor = grayColor * SEPIA;
            finalColor = sepiaColor;
        }
        
        
        //again we'll use mix so that the sepia effect is at 75%
        texColour.rgb = mix(texColour.rgb, finalColor, 0.75f);
        ////
    }
    
    texColour.rgb *= vignette;
    vOutputColour = texColour ;
    
}
