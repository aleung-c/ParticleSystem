#include "../../../includes/ParticleSystem.hpp"

void		BoundingBoxComponent::InitBBoxDisplay()
{
	// Cube 1x1x1, centered on origin
	// GLfloat vertices[] = {
	// 	-0.5, -0.5, -0.5, 1.0,
	// 	0.5, -0.5, -0.5, 1.0,
	// 	0.5,  0.5, -0.5, 1.0,
	// 	-0.5,  0.5, -0.5, 1.0,
	// 	-0.5, -0.5,  0.5, 1.0,
	// 	0.5, -0.5,  0.5, 1.0,
	// 	0.5,  0.5,  0.5, 1.0,
	// 	-0.5,  0.5,  0.5, 1.0,
	// };

		// ------ Bounding box
	LocalCenter.x = ((LocalMin.x + LocalMax.x) / 2.0);
	LocalCenter.y = ((LocalMin.y + LocalMax.y) / 2.0);
	LocalCenter.z = ((LocalMin.z + LocalMax.z) / 2.0);
	LocalCenter.w = 1.0;

	Width = LocalMax.x - LocalMin.x;
	Height = LocalMax.y - LocalMin.y;
	Depth = LocalMax.z - LocalMin.z;

	BoxVertices.push_back(glm::vec4(LocalMin.x, LocalMin.y, LocalMin.z, 1.0));

	BoxVertices.push_back(glm::vec4(LocalMax.x, LocalMin.y, LocalMin.z, 1.0));
	BoxVertices.push_back(glm::vec4(LocalMax.x, LocalMax.y, LocalMin.z, 1.0));

	BoxVertices.push_back(glm::vec4(LocalMax.x, LocalMax.y, LocalMax.z, 1.0));

	BoxVertices.push_back(glm::vec4(LocalMin.x, LocalMax.y, LocalMax.z, 1.0));
	BoxVertices.push_back(glm::vec4(LocalMin.x, LocalMin.y, LocalMax.z, 1.0));

	BoxVertices.push_back(glm::vec4(LocalMax.x, LocalMin.y, LocalMax.z, 1.0));
	BoxVertices.push_back(glm::vec4(LocalMin.x, LocalMax.y, LocalMin.z, 1.0));


	Min = LocalMin;
	Max = LocalMax;
	Center = LocalCenter;

	glGenBuffers(1, &_bbox_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _bbox_vbo);
	glBufferData(GL_ARRAY_BUFFER, BoxVertices.size() * sizeof(glm::vec4), &BoxVertices[0], GL_STATIC_DRAW);
}

void		BoundingBoxComponent::Update(glm::mat4 &transform)
{

	// std::cout << "old min = " << Min.x << "x " << Min.y << "y " << Min.z << "z" << std::endl; 
	Min = glm::vec4(9999.0, 9999.0, 9999.0, 1.0);
	Max = glm::vec4(-9999.0, -9999.0, -9999.0, 1.0);

	for (std::vector<glm::vec4>::iterator it = BoxVertices.begin();
			it != BoxVertices.end(); it++)
	{
		(*it) = transform * (*it);
		if ((*it).x >= Max.x)
			Max.x = (*it).x;
		if ((*it).x <= Min.x)
			Min.x = (*it).x;
		// Set y
		if ((*it).y >= Max.y)
			Max.y = (*it).y;
		if ((*it).y <= Min.y)
			Min.y = (*it).y;
		// Set z
		if ((*it).z >= Max.z)
			Max.z = (*it).z;
		if ((*it).z <= Min.z)
			Min.z = (*it).z;
	}
	Center.x = ((Min.x + Max.x) / 2.0);
	Center.y = ((Min.y + Max.y) / 2.0);
	Center.z = ((Min.z + Max.z) / 2.0);
	Center.w = 1.0;
	// std::cout << "new min = " << Min.x << "x " << Min.y << "y " << Min.z << "z" << std::endl << std::endl;

	// glBindBuffer(GL_ARRAY_BUFFER, _bbox_vbo);
	// glBufferData(GL_ARRAY_BUFFER, BoxVertices.size() * sizeof(glm::vec4), &BoxVertices[0], GL_STATIC_DRAW);
}

GLuint		BoundingBoxComponent::GetVbo()
{
	return (_bbox_vbo);
}

GLuint		BoundingBoxComponent::GetVao()
{
	return (_bbox_vao);
}
