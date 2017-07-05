#version 410

// ----- Host set variables
uniform bool			has_texture = false;
uniform bool			particle_coloring = false;
uniform sampler2D		texture_0;

// ----- Incoming from vertex shader
in vec2					texture_coordinates;
in float				dist_to_grav_point;

// ----- OUT COLOR -----
out vec4				frag_colour;
// ----- OUT COLOR -----

void main ()
{
	vec4	texel;
	float	r;
	float	g;
	float	b;
	
	if (has_texture == true)
	{
		texel = texture(texture_0, texture_coordinates);
		frag_colour = texel;
	}
	else
	{
		if (particle_coloring)
		{
			r = 0.0 + dist_to_grav_point / 40.0;
			g = 1.0 - dist_to_grav_point / 40.0;
			b = 0.0 + dist_to_grav_point / 4.0;
			frag_colour = vec4(r, g, b, 1.0);
		}
		else
		{
			// just green will do.
			frag_colour = vec4(0.0, 1.0, 0.0, 1.0);
		}
	}
}
