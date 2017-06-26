#include "../../../includes/ParticleSystem.hpp"

/*
** The loader for our obj files.
*/

void		GameObject::getObjValues(FILE *file)
{
	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break ;
		}
		else
		{
			if (strncmp(lineHeader, "v", 3) == 0)
			{
				glm::vec4	vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertex.w = 1.0;
				// ----- Set Bounding box values;
				// Set x
				if (vertex.x >= BoundingBox.LocalMax.x)
					BoundingBox.LocalMax.x = vertex.x;
				if (vertex.x <= BoundingBox.LocalMin.x)
					BoundingBox.LocalMin.x = vertex.x;
				// Set y
				if (vertex.y >= BoundingBox.LocalMax.y)
					BoundingBox.LocalMax.y = vertex.y;
				if (vertex.y <= BoundingBox.LocalMin.y)
					BoundingBox.LocalMin.y = vertex.y;
				// Set z
				if (vertex.z >= BoundingBox.LocalMax.z)
					BoundingBox.LocalMax.z = vertex.z;
				if (vertex.z <= BoundingBox.LocalMin.z)
					BoundingBox.LocalMin.z = vertex.z;
				// ----- end set bounding box min max.
				_objVertices.push_back(vertex);
			}
			else if (strncmp(lineHeader, "vt", 3) == 0)
			{
				glm::vec2	uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y );
				_objUVs.push_back(uv);
			}
			else if (strncmp(lineHeader, "vn", 3) == 0)
			{
				glm::vec3	normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				_objNormals.push_back(normal);
			}
			else if (strncmp(lineHeader, "f", 3) == 0)
			{
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (matches != 9)
				{
					printf("Cant read file: OBJ parser error\n");
					// std::cout << file << std::endl;
					return ;
				}
				_objVertexIndices.push_back(vertexIndex[0]);
				_objVertexIndices.push_back(vertexIndex[1]);
				_objVertexIndices.push_back(vertexIndex[2]);
				_objUVIndices.push_back(uvIndex[0]);
				_objUVIndices.push_back(uvIndex[1]);
				_objUVIndices.push_back(uvIndex[2]);
				_objNormalIndices.push_back(normalIndex[0]);
				_objNormalIndices.push_back(normalIndex[1]);
				_objNormalIndices.push_back(normalIndex[2]);
			}
		}
	}
}
