#version 410

in vec4						vp;

// model matrices
uniform mat4				mvp_matrix;

layout(location = 1) in		vec2 uv;

// layout(location = 1) in		vec3 v_normal;
// layout(location = 2) in		vec3 v_color;
// layout(location = 4) in		vec2 uv;
// layout(location = 5) in		int transition_flag;

// out vec4					vertex_position;
out vec2					texture_coordinates;
// out vec3					vertex_normal;
// flat out vec3			color;

// flat out int				t_flag; // "flat" means not interpolated.

void main ()
{
	gl_Position = mvp_matrix * vp;

	// variables to send to the fragment shader.
	// vertex_normal = v_normal;
	// color = v_color;
	texture_coordinates = uv;
	// t_flag = transition_flag;
}
