#version 410

// in vec4					vertex_position;
// in vec3					vertex_normal;
// flat in vec3			color;
in vec2					texture_coordinates;
// flat in int				t_flag;


// uniform int				texture_id = 0;
uniform bool			has_texture = false;
uniform sampler2D		texture_0;
// uniform sampler2D		texture_1;

// the final color the fragment will take.
out vec4				frag_colour;

void main ()
{
	vec4	texel;
	
	if (has_texture == true)
	{
		texel = texture(texture_0, texture_coordinates);
		frag_colour = texel;
	}
	else
	{
		frag_colour = vec4(0.0, 1.0, 0.0, 1.0);
	}
}
