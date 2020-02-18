#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

uniform sampler2D texture_1;
uniform vec2 size;
uniform float terrain_scale;

out vec2 texcoord;
out vec3 world_position;
out vec3 world_normal;
out float out_height;

void main()
{
	texcoord = vec2(v_position.x, v_position.z);
	//calcul inaltime
	float height = texture2D(texture_1, v_texture_coord).r;
	out_height = height;

	//calcul normala
	vec2 texelSize = vec2(1.0/size.x, 1.0/size.y);
	float heightRight = texture2D(texture_1, vec2(v_texture_coord.x+texelSize.x,v_texture_coord.y)).r;
	float heightUp = texture2D(texture_1, vec2(v_texture_coord.x,v_texture_coord.y + texelSize.y)).r;
	float Hx = height - heightRight;
	float Hz = height - heightUp;
	vec3 new_normal = normalize(vec3(Hx,1,Hz));

	//calcul pozitie globala si normala gloabala
	world_position = (Model * vec4(v_position,1)).xyz;
	world_normal = normalize( mat3(Model) * new_normal );


	gl_Position = Projection * View * Model * vec4(v_position.x, height* terrain_scale, v_position.z, 1.0);
}
