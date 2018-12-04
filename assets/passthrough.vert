#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
         
smooth out vec3 color; 
out vec3 fragPos;
          
uniform mat4 projectionMatrix; 
uniform mat4 lightMatrix;
uniform mat4 modelMatrix;
        
void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  fragPos = ((projectionMatrix * lightMatrix * modelMatrix) * v).xyz;
  gl_Position = (projectionMatrix * lightMatrix * modelMatrix) * v;
  color = v_color; 
}
          
