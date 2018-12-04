#version 330

smooth in vec2 texCoord;
uniform sampler2D prePass;

out vec4 frag_color;
const float PI = 3.1415926535;
float random(vec3 scale,float seed){return fract(sin(dot(gl_FragCoord.xyz+seed,scale))*43758.5453+seed);}

vec2 fisheye(vec2 coordinates, float power) {
	//offset
	vec2 cc = coordinates - 0.5;
	float dist = dot(cc, cc);
	return coordinates + cc * dist * power;
}

void main(void)
{
	//passthrough
	frag_color = texture(prePass, texCoord);

	//invert colors
	//frag_color = 1-texture(prePass, texCoord);

	//screen door
	//frag_color = texture(prePass, vec2(texCoord.x, texCoord.y-0.01*(sin(80*texCoord.y))));

	//color grade
/*	vec4 col = texture(prePass, texCoord);
	vec3 dark = vec3(0.0,0.0,0.3);
	vec3 bright = vec3(1.0,0.3,0.0);

	//generate luminance map
	float luminance = (col.x + col.y + col.z)/3.0;
	vec3 final = 1 - (1 - col.rgb) * (1 - (mix(dark, bright, luminance)));

	frag_color = vec4(final, 1.0);*/

	//box blur
/*	vec2 delta = vec2(0.05,0.05);
	vec4 color=vec4(0.0);
	float total=0.0;
	float offset=random(vec3(12.9898,78.233,151.7182),0.0);
	for(float t=-10.0;t<=10.0;t++){
		float percent=(t+offset-0.5)/30.0;
		float weight=1.0-abs(percent);
		vec4 sample=texture2D(prePass,texCoord+delta*percent);
		sample.rgb*=sample.a;
		color+=sample*weight;
		total+=weight;
	}

	frag_color=color/total;
	frag_color.rgb /= frag_color.a+0.00001;*/

	//bloom
/*	vec2 delta = vec2(0.08,0.08);
	vec4 color=vec4(0.0);
	float total=0.0;
	float offset=random(vec3(12.9898,78.233,151.7182),0.0);
	for(float t=-10.0;t<=10.0;t++){
		float percent=(t+offset-0.5)/30.0;
		float weight=1.0-abs(percent);
		vec4 sample=max(vec4(0,0,0,1), texture2D(prePass,texCoord+delta*percent));
		sample = pow(sample, vec4(7,7,7,1));
		sample.rgb*=sample.a;
		color+=sample*weight;
		total+=weight;
	}

	frag_color=color/total;
	frag_color.rgb /= frag_color.a+0.00001;
	frag_color.rgb += texture2D(prePass,texCoord).rgb;*/

	//fisheye
	//frag_color = texture(prePass, fisheye(texCoord, -2.0));

	//chromatic abberation
/*	frag_color.r = texture(prePass, fisheye(texCoord, -2.1)).r;
	frag_color.g = texture(prePass, fisheye(texCoord, -2.2)).g;
	frag_color.b = texture(prePass, fisheye(texCoord, -2.3)).b;*/

}
