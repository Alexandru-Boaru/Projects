#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform float Random;
uniform vec3 object_color;

// TODO: output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_texture;
out vec3 frag_color;

mat4 rotationX( in float angle ) {

	return mat4(	1.0,		0,			0,			0,

			 		0, 	cos(angle),	-sin(angle),		0,

					0, 	sin(angle),	 cos(angle),		0,

					0, 			0,			  0, 		1);

}



mat4 rotationY( in float angle ) {

	return mat4(	cos(angle),		0,		sin(angle),	0,

			 				0,		1.0,			 0,	0,

					-sin(angle),	0,		cos(angle),	0,

							0, 		0,				0,	1);

}



mat4 rotationZ( in float angle ) {

	return mat4(	cos(angle),		-sin(angle),	0,	0,

			 		sin(angle),		cos(angle),		0,	0,

							0,				0,		1,	0,

							0,				0,		0,	1);

}

void main()
{

	float a = v_normal.x * Random;
	float b = v_normal.y * Random;

	vec4 vertex = vec4(v_position.xyz, 1.0);
	vertex = vertex * rotationZ(Time);
	

	frag_position = v_position;
	frag_normal = v_normal;
	frag_texture = v_texture;
	frag_color = object_color;
	gl_Position = Projection * View * Model*vec4(vertex.xyz, 1.0);
}
