__kernel void place_particles(__global float4 *vertices, __global float4 *origine)
{
	int base = get_global_id(0);

	// vertices[base + 0] += 0.01;
	// vertices[base + 1] += 0.01;
	// vertices[base + 2] += 0.01;

	vertices[base] =  (float4)
						(vertices[base].x + 0.01,
						vertices[base].y + 0.01,
						vertices[base].z + 0.01,
						1.0f);
}
