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


void main()
{	

	frag_position = v_position;
	frag_normal = v_normal;
	frag_texture = v_texture;
	frag_color = object_color;
	gl_Position = Projection * View * Model*vec4(v_position, 1.0);
}
