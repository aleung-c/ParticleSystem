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
**	are not in the sphere, with a reversed vector displacement.
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

__kernel void	animate_particles(__global float4 *vertices, //	0
								__global float *distances, //	1
								__global float4 *origine, //	2
								float speed, //					3
								float x_gravity_point, //		4
								float y_gravity_point, //		5
								float x_mouse_point, //			6
								float y_mouse_point, //			7
								int particle_status) //			8

{
	__private int		base = get_global_id(0);
	float4				velocity;
	float				dist;
	float4				normal;
	float4				tangent;
	float4				cross_angle;

	if (particle_status == 1)
	{
		velocity.x = x_gravity_point - vertices[base].x;
		velocity.y = y_gravity_point - vertices[base].y;
		velocity.z = -vertices[base].z;
		velocity = normalize(velocity);
	}
	else if (particle_status == 2)
	{
		velocity.x = vertices[base].x - x_gravity_point;
		velocity.y = vertices[base].y - y_gravity_point;
		velocity.z = vertices[base].z;
		velocity = normalize(velocity);
	}
	else
	{
		normal.x = vertices[base].x - x_gravity_point;
		normal.y = vertices[base].y - y_gravity_point;
		normal.z = vertices[base].z;

		cross_angle.x = 0.0;
		cross_angle.y = 0.0;
		cross_angle.z = vertices[base].z + 0.01;
		tangent = cross(normal, cross_angle);
		velocity = normalize(tangent);
	}

	// Used for coloring with distance mouse-particle!
	distances[base] = sqrt(pow(x_mouse_point - vertices[base].x, 2)
				+ pow(y_mouse_point - vertices[base].y, 2)
				+ pow(- vertices[base].z, 2));

	// new position for the current particle
	vertices[base] = vertices[base] + velocity * speed;
}
