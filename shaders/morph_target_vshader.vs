#version 410

/*
**	This shader handle the interpolation for morph
**	target animation. It will make the interpolation from
**	vp to target_frame according to time, modified outside.
*/

in vec4						vp; // location = 0

uniform float				time;

// model matrices
uniform mat4				mvp_matrix;

layout(location = 1) in		vec2 uv;
layout(location = 2) in		vec4 target_frame_vp;

out vec2					texture_coordinates;


void main ()
{
	vec4	interpolated_vp;

	interpolated_vp = vp + (target_frame_vp - vp) * time;
	gl_Position = mvp_matrix * interpolated_vp;

	// set final vertex pos;
	// vertex_position = gl_Position;

	// variables to send to the fragment shader.
	// vertex_normal = v_normal;
	// color = v_color;
	texture_coordinates = uv;
	// t_flag = transition_flag;
}