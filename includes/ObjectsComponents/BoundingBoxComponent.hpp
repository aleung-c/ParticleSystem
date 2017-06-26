#ifndef BOUNDINGBOXCOMPONENT_HPP
# define BOUNDINGBOXCOMPONENT_HPP

#include "../ParticleSystem.hpp"

class BoundingBoxComponent
{
	public:
		// World pos must be vec4 !!
		glm::vec4						LocalMin;
		glm::vec4						LocalMax;
		glm::vec4						LocalCenter;

		glm::vec4						Min;
		glm::vec4						Max;
		glm::vec4						Center;

		float							Width;
		float							Height;
		float							Depth;

		// opengl display bounding box;
		GLuint							_bbox_vao;
		GLuint							_bbox_vbo;

		std::vector<glm::vec4>			BoxVertices;

		void							InitBBoxDisplay();
		void							Update(glm::mat4 &transform);

		GLuint							GetVbo();
		GLuint							GetVao();
};

#endif
