__kernel void	place_particles_cubic(__global float4 *vertices, __global float4 *origine,
								float radius, __global double *rand_suite)
{
	int			base;
	float3		new_pos;

	base = get_global_id(0);

	// cubic placement.
	new_pos.x = 0.0 - rand_suite[base];
	new_pos.y = 0.0 - rand_suite[base + 1];
	new_pos.z = 0.0 - rand_suite[base + 2];

	vertices[base] =  (float4)(new_pos.x, new_pos.y, new_pos.z, 1.0f);
}

/*
**	Placing the particle in a sphere with a bad method: i just put back the points that 
**	are not in the sphere, with a vector displacement.
*/

__kernel void	place_particles_spheric(__global float4 *vertices, __global float4 *origine,
								float radius, __global double *rand_suite)
{
	int			base;
	float3		new_pos;
	float3		replace_sphere;

	base = get_global_id(0);

	new_pos.x = 0.0 - rand_suite[base];
	new_pos.y = 0.0 - rand_suite[base + 1];
	new_pos.z = 0.0 - rand_suite[base + 2];

	replace_sphere.x = 0.0;
	replace_sphere.y = 0.0;
	replace_sphere.z = 0.0;

	// IF point is NOT IN SPHERE
	if (pow(new_pos.x - 0.0, 2) + pow(new_pos.y - 0.0, 2) + pow(new_pos.z - 0.0, 2) > pow(radius, 2))
	{
		// move the point in the opposite dirrection.
		new_pos = new_pos + normalize(replace_sphere - new_pos) * radius;
	}
	vertices[base] = (float4)(new_pos.x, new_pos.y, new_pos.z, 1.0f);
}

__kernel void	animate_particles(__global float4 *vertices, __global double *rand_suite,
								__global float4 *origine,
								float radius, float speed,
								float x_gravity_point, float y_gravity_point)
{
	int					base;
	float3				new_pos;
	float3				particle_pos;
	float3				target_pos;
	float3				vec_dir;
	float				dist;
	__private bool		reached;

	base = get_global_id(0);
	// position of the current particle
	particle_pos.x = vertices[base].x;
	particle_pos.y = vertices[base].y;
	particle_pos.z = vertices[base].z;

	//position of the target (ie mouse world position)
	target_pos.x = x_gravity_point;
	target_pos.y = y_gravity_point;
	target_pos.z = 0.0;

	dist = sqrt(pow(target_pos.x - particle_pos.x, 2)
				+ pow(target_pos.y - particle_pos.y, 2)
				+ pow(target_pos.z - particle_pos.z, 2));

	if (reached == false)
	{
		vec_dir = normalize(target_pos - particle_pos);
		if (dist < 0.01)
			reached = true;
	}
	else
	{
		target_pos.x = x_gravity_point - rand_suite[base];
		target_pos.y = y_gravity_point - rand_suite[base + 1];
		target_pos.y = vertices[base].z - rand_suite[base + 2];
		vec_dir = normalize(target_pos - particle_pos);

		// recheck distance to radius around.
		dist = sqrt(pow(target_pos.x - particle_pos.x, 2)
			+ pow(target_pos.y - particle_pos.y, 2)
			+ pow(target_pos.z - particle_pos.z, 2));
		if (dist < 0.01)
			reached = false;
	}
	
	// new position for the current particle
	new_pos = particle_pos + vec_dir * speed;
	vertices[base] = (float4)(new_pos.x, new_pos.y, new_pos.z, 1.0f);
}

// __kernel void	update_distance(__global float4 *vertices, __global float4 *distances,
// 								float x_gravity_point, float y_gravity_point)
// {
// 	int					base;
// 	float3				vert_pos;
// 	float3				grav_point_pos;

// 	base = get_global_id(0);
// 	vert_pos = (float3)(vertices[base]);
// 	grav_point_pos = (float3)(x_gravity_point, y_gravity_point, 0.0);
// 	distances[base] = sqrt(pow(vert_pos.x - grav_point_pos.x, 2)
// 			+ pow(vert_pos.y - grav_point_pos.y, 2)
// 			+ pow(vert_pos.z - grav_point_pos.z, 2));
// }
