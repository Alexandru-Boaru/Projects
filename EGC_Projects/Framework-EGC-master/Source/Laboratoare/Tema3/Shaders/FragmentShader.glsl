#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int mode;

uniform vec3 light_direction;
uniform vec3 light_position;

uniform float cut_off;
uniform int is_spot_light;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 spot_positions[2];
uniform vec3 spot_directions[2];
uniform vec3 light_colors[2];
uniform vec4 ver_color;

in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;



layout(location = 0) out vec4 out_color;

void main()
{
	vec4 temp_color = vec4(1);
	if(mode==2){
		vec4 color1 = texture2D(texture_1, texcoord);
		vec4 color2 = texture2D(texture_2, texcoord);
		float alpha = color1.w;
		if (alpha < 0.5) {
			discard;
		}
		vec3 colormix = mix(color1.xyz, color2.xyz, 0.5f);
		temp_color = vec4(colormix.xyz, color1.w);
	}
	else if(mode==1 || mode ==3){
		vec4 color1 = texture2D(texture_1, texcoord);
		float alpha = color1.w;
		if (alpha < 0.5) {
			discard;
		}
		temp_color = color1;
	}
	

	vec3 N = normalize (world_normal);
	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	vec3 R = reflect(-L, N);
	float spot_light = dot(-L, light_direction);
	

	if(spot_light > cos(cut_off) || is_spot_light == 1){

		float ambient_light = 0.75;


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
			color = (ambient_light + diffuse_light + specular_light) * temp_color.xyz * light_atten * 1;
		}
		else{
			color = (ambient_light + diffuse_light + specular_light) * temp_color.xyz * light_att_factor * 1;
		}

		out_color = vec4(color.xyz,1);
	}
	else{
		out_color = vec4(0);
	}

	for(int i = 0; i < 2; i++){
		N = normalize (world_normal);
		L = normalize( spot_positions[i] - world_position );
		V = normalize( eye_position - world_position );
		H = normalize( L + V );
		R = reflect(-L, N);
		spot_light = dot(-L, spot_directions[i]);
	

		if(spot_light > cos(cut_off)){

			float ambient_light = 0.75;


			float NdotL = dot(N,L);
			float diffuse_light = material_kd * 1 * max(NdotL, 0);

			float specular_light = 0;

			if (NdotL > 0)
			{
				specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);
			}

			float d = length(world_position - spot_positions[i]);
			float light_atten = 1/(0.01*d*d + 0.02*d + 1);
			float spot_light_limit = cos(cut_off);

			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			vec3 color;
			
			color = (ambient_light + diffuse_light + specular_light) * temp_color.xyz * light_colors[i] * light_att_factor * 1;

			out_color = vec4(out_color.xyz+color.xyz,1);
		}
		else{
			out_color += vec4(0);
		}
	}
	if (mode == 0){
		out_color = ver_color;
	}
	if(mode == 3){
		if(world_position.y < 0){
			discard;
		}
		out_color = temp_color;
	}
	//out_color = color;
	
}