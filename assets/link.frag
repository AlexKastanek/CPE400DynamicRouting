#version 330

smooth in vec3 color;
out vec4 frag_color;
uniform vec2 lineColor;

void main(void) {
	vec3 bestColor = vec3(0,1,0);
	vec3 worstColor = vec3(1,0,0);
	vec3 shortestPathColor = vec3(1,1,0);

	vec3 finalColor;
	//x channel used for rating line width
	finalColor = mix(bestColor, worstColor, lineColor.x);

	//y channel used for indicating shortest path
	finalColor = mix(finalColor, shortestPathColor, lineColor.y);

	frag_color = vec4(finalColor, 1.0);
}
