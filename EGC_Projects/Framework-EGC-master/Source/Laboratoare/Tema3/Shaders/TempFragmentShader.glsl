#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int use_second;
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec4 color = vec4(1);
	if(use_second==1){
		vec4 color1 = texture2D(texture_1, texcoord);
		vec4 color2 = texture2D(texture_2, texcoord);
		float alpha = color1.w;
		if (alpha < 0.5) {
			discard;
		}
		vec3 colormix = mix(color1.xyz, color2.xyz, 0.5f);
		color = vec4(colormix.xyz, 0.25);
	}
	else{
		vec4 color1 = texture2D(texture_1, texcoord);
		float alpha = color1.w;
		if (alpha < 0.5) {
			discard;
		}
		color = color1;
	}
	out_color = color;
	
}