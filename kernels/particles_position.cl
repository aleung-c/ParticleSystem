__kernel void place_particles(__global float4 *vertices, __global float4 *origine)
{
	int			base;
	float		new_x_value;
	float		new_y_value;
	float		new_z_value;

	base = get_global_id(0);
	new_x_value = vertices[base].x + 0.01 * base;
	new_x_value = vertices[base].y + 0.01 * base;
	new_x_value = vertices[base].z + 0.01 * base;

	vertices[base] =  (float4)(new_x_value, new_y_value, new_z_value, 1.0f);
}
