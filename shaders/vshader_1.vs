#version 410

// ----- Host set variables
uniform bool						particle_coloring = false;
uniform mat4						mvp_matrix;

in vec4								vp;
layout(location = 1) in vec2		uv;
layout(location = 2) in float		dist;

// ----- Out to fragment shader.
out vec2							texture_coordinates;
out float							dist_to_grav_point;

void main ()
{
	gl_Position = mvp_matrix * vp;

	texture_coordinates = uv;
	dist_to_grav_point = dist;
}
