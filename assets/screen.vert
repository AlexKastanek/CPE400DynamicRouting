#version 330
layout (location = 0) in vec3 v_position;

smooth out vec2 texCoord;
         
void main(void) 
{ 
  //passthrough with no matrices
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = v; 

  //convert screen space coordinates (-1.0 -> +1.0) to texture coordinates (0.0 -> 1.0)
  texCoord = (v_position.xy+1.0)/2;
}
          
