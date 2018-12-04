#version 330

smooth in vec3 color;
in vec3 fragPos;
in vec4 lightSpaceFragPos;

out vec4 frag_color;

uniform mat4 modelMatrix;
uniform sampler2D shadowMap;

float shadowCalc(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; 
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;  
    float bias = 0.00004;
    float shadow = currentDepth - bias > closestDepth  ? 0.0 : 1.0;
    return shadow;
}

void main(void)
{
   vec3 lightPos = vec3(3,5,-2);
   vec3 N = normalize((modelMatrix * vec4(color, 1.0)).xyz);
   vec3 L = normalize(lightPos - fragPos);
   vec3 objColor = (color + vec3(1))/2.0;
   float diff = dot(N, L);
   float shadow = shadowCalc(lightSpaceFragPos);
   float amb = 0.1;
   vec3 finalLight = (max(0.0, diff*shadow) + amb)*vec3(1.0f);
   frag_color = vec4(finalLight*objColor, 1.0);
}
