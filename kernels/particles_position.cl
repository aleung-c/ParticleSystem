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

__kernel void	animate_particles(__global float4 *vertices, //	0
								__global float *distances, //	1
								__global double *rand_suite, //	2
								__global float4 *origine, //	3
								float radius, //				4
								float speed, //					5
								float x_gravity_point, //		6
								float y_gravity_point, //		7
								float x_mouse_point, //			8
								float y_mouse_point, //			9
								float radial_accel) //			10

{
	int					base;
	float3				new_pos;
	float3				particle_pos;
	// float3				target_pos;
	float3				velocity;
	float				dist;
	__private float		angle_val_loc = 0.0;

	base = get_global_id(0);
	// position of the current particle
	particle_pos.x = vertices[base].x;
	particle_pos.y = vertices[base].y;
	particle_pos.z = vertices[base].z;

	float3				normal;
	float3				tangent;
	float3				cross_angle;

	normal.x = particle_pos.x - x_gravity_point;
	normal.y = particle_pos.y - y_gravity_point;
	normal.z = particle_pos.z;

	cross_angle.x = 0.0;
	cross_angle.y = 0.0;
	cross_angle.z = particle_pos.z;

	tangent = cross(normal, cross_angle);

	// Used for coloring with distance mouse-particle!
	distances[base] = sqrt(pow(x_mouse_point - particle_pos.x, 2)
				+ pow(y_mouse_point - particle_pos.y, 2)
				+ pow(0.0 - particle_pos.z, 2));
	// distances[base] = dist;

	velocity = normalize(tangent);

	// new position for the current particle
	new_pos = particle_pos + velocity * speed;
	vertices[base] = (float4)(new_pos.x, new_pos.y, new_pos.z, 1.0f);
}

	//position of the target (ie mouse world position) -> fonctionel;
	// target_pos.x = x_gravity_point;
	// target_pos.y = y_gravity_point;
	// target_pos.z = 0.0;

	// orbit 2D -> fonctionnel;
	// angle_val_loc += 0.1;
	// target_pos.x = x_gravity_point + (particle_pos.x - x_gravity_point) * cos(angle_val_loc)
	// 				- (particle_pos.y - y_gravity_point) * sin(angle_val_loc);
	// target_pos.y = y_gravity_point + (particle_pos.x - x_gravity_point) * sin(angle_val_loc)
	// 				+ (particle_pos.y - y_gravity_point) * cos(angle_val_loc);
	// target_pos.z = 0.0;

	// orbit 3D tests; -> semi fonctionnel.
	// angle_val_loc += 0.1;
	// // ----- translation mouse pose ->>> particle pos.
	// target_pos.x = x_gravity_point + (particle_pos.x - x_gravity_point);
	// target_pos.y = y_gravity_point + (particle_pos.y - y_gravity_point);
	// target_pos.z = 0.0 + particle_pos.z;
	// ----- rotation XYZ
	// rotation X
	// target_pos.y = cos(angle_val_loc) * target_pos.y + -sin(angle_val_loc) * target_pos.z;
	// target_pos.z = sin(angle_val_loc) * target_pos.y + cos(angle_val_loc) * target_pos.z; 
	// // rotation Y
	// target_pos.x = cos(angle_val_loc) * target_pos.x + sin(angle_val_loc) * target_pos.z;
	// target_pos.z = -sin(angle_val_loc) * target_pos.x + cos(angle_val_loc) * target_pos.z;
	// rotation Z
	// target_pos.x = cos(angle_val_loc) * target_pos.x + -sin(angle_val_loc) * target_pos.y;
	// target_pos.y = sin(angle_val_loc) * target_pos.x + cos(angle_val_loc) * target_pos.y;
	// // ----- translation particle pos ->>> mouse pos
	// target_pos.x -= (particle_pos.x - x_gravity_point);
	// target_pos.y -= (particle_pos.y - y_gravity_point);
	// target_pos.z -= particle_pos.z;
