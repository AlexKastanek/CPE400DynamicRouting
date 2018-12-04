#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
         
smooth out vec3 color; 
out vec3 fragPos;
out vec4 lightSpaceFragPos;
out vec3 lightPos;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightMatrix;
        
void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  fragPos = ((modelMatrix) * v).xyz;
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  color = v_color; 
  lightSpaceFragPos = (projectionMatrix * lightMatrix * vec4(fragPos, 1.0));
  lightPos = (inverse(lightMatrix) * vec4(0,0,0,1)).xyz;
}
          
