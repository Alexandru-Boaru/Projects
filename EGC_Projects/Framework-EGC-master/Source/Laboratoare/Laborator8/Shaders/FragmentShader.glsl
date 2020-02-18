#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform float cut_off;
uniform int is_spot_light;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{

	
	vec3 N = normalize (world_normal);
	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	vec3 R = reflect(-L,N);

	float spot_light = dot(-L, light_direction);
	

	if(spot_light > cos(cut_off) || is_spot_light == 1){

		float ambient_light = 0.25;


		float NdotL = dot(N,L);
		float diffuse_light = material_kd * 1 * max(NdotL, 0);

		float specular_light = 0;

		if (NdotL > 0)
		{
			specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);
		}

		float d = length(world_position - light_position);
		float light_atten = 1/(0.01*d*d + 0.02*d + 1);
		float spot_light_limit = cos(cut_off);

		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);
		vec3 color;
		if(is_spot_light==1){
			color = (ambient_light + diffuse_light + specular_light) * object_color * light_atten * 1;
		}
		else{
			color = (ambient_light + diffuse_light + specular_light) * object_color * light_att_factor * 1;
		}

		out_color = vec4(color.xyz,1);
	}
	else{
		out_color = vec4(0);
	}
}