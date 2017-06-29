__kernel void place_particles(__global float4 *vertices, __global float4 *origine,
								float radius, __global double *rand_suite)
{
	int			base;
	float		new_x_value;
	float		new_y_value;
	float		new_z_value;

	base = get_global_id(0);
	// cubic placement.
	new_x_value = vertices[base].x + rand_suite[base];
	new_y_value = vertices[base].y + rand_suite[base + 1];
	new_z_value = vertices[base].z + rand_suite[base + 2];

	vertices[base] =  (float4)(new_x_value, new_y_value, new_z_value, 1.0f);
}

__kernel void animate_particles(__global float4 *vertices, __global float4 *origine,
								float radius, float x_gravity_point, float y_gravity_point)
{
	int			base;
	float		new_x_value;
	float		new_y_value;
	float		new_z_value;

	base = get_global_id(0);
	// new_x_value = vertices[base].x + rand_suite[base];
	// new_y_value = vertices[base].y + rand_suite[base];
	// new_z_value = vertices[base].z + rand_suite[base];

	vertices[base] = (float4)(new_x_value, new_y_value, new_z_value, 1.0f);
}
