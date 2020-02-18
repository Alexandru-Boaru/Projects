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
uniform vec3 second_object_color;

uniform vec3 object_center;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform float material_shininess;

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
	// TODO: send output to fragment shader
	//vec3 rot = rotationX(Time);



	float a = v_normal.x * Random;
	float b = v_normal.y * Random;

	vec4 vertex = vec4(a, b, 1.0, 1.0);
	vertex = vertex * rotationZ(Time);


	frag_position = v_position;
	frag_normal = v_normal;
	frag_texture = v_texture;

	
	//if(floatBitsToInt(v_normal.y) % 4 == 0){
	//	frag_color = second_object_color;
	//}
	//else{
	//	frag_color = object_color;
	//}
	//frag_color = v_color;
	// TODO: compute gl_Position
	gl_Position = Projection * View * Model*vec4(vec3(v_position.x + vertex.x, 
														v_position.y + vertex.y, 
														v_position.z + v_normal.z * Random), 1.0);

	// TODO: compute world space vectors
	vec3 new_position = v_position + vertex.xyz * vec3(Random);
	
	vec3 world_pos = (Model * vec4(new_position,1)).xyz;
	vec3 new_normal = normalize(world_pos - object_center);
	vec3 N = normalize( mat3(Model) * new_normal );
	vec3 L = normalize( light_position - world_pos );
	vec3 V = normalize( eye_position - world_pos );
	vec3 H = normalize( L + V );
	vec3 R = reflect(-L,N);


	// TODO: define ambient light component
	//float ambient_light = 0.25;
	float ambient_light = material_kd * 15;

	// TODO: compute diffuse light component
	//float diffuse_light = 0;
	float NdotL = dot(N,L);
	float diffuse_light = material_kd * 1 * max(NdotL, 0);

	// TODO: compute specular light component
	//float specular_light = 0;
	float specular_light = 0;

	//float specular_light = material_ks * 1 * get_light * pow(max(dot(N, H), 0), material_shininess);

	if (NdotL > 0)
	{
		specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
	}

	float d = length(world_pos - light_position);
	float light_atten = 1/(0.01*d*d + 0.02*d + 1);

	// TODO: compute light
	vec3 new_color;
	if(floatBitsToInt(v_normal.y) % 40 == 0){
		new_color = second_object_color;
	}
	else{
		new_color = object_color;
	}
	vec3 out_color = /*vec3(float(254.0 / 255.0), float(192.0 / 255.0), float(158.0 / 255.0))**/(ambient_light + diffuse_light + specular_light) * new_color * light_atten * 1;

	// TODO: send color light output to fragment shader
	
	frag_color = out_color;
}
