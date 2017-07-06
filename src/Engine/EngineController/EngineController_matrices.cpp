#include "../../../includes/ParticleSystem.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Matrices															//
//	This creates the usual MODEL - VIEW - PROJECTION					//
//	matrice.															//
//																		//
// --------------------------------------------------------------------	//

/*
**	This method initialize the VIEW and PROJECTION matrices.
**	The MODEL matrice will be set for each object at runtime.
**	Same goes for the merge into the MVP matrice.
*/

void		EngineController::loadMatrices()
{
	// View matrices init;
	MainCamera = new GameObject("MainCamera");
	MainCamera->Transform.Position = glm::vec3(0.0, 5.0, 10.0);
	MatView = glm::lookAt(
		MainCamera->Transform.Position,
		CameraLookAt, // regarde l'origine
		glm::vec3(0.0, 2.0, 0.0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
	);

	// Project matrices init;
	CameraNear = 0.1;
	CameraFar = 300.0;
	CameraFov = 45.0;
	CameraAspect = 1.77; // 4/3, 16/9, etc 1 = 4/4
	MatPerspectiveProjection = glm::perspective(CameraFov, CameraAspect, CameraNear, CameraFar);

	MatOrthographicProjection = glm::ortho(0.0f, (float)WindowWidth, 0.0f, (float)WindowHeight);
}

/*
**	For one object of our scene, apply model view projection matrices.
**	The rotation pivot point is correct, just make sure your
**	obj model is exported from its local center.
*/

void		EngineController::applyMatricesToObject(GameObject *Object)
{
	// Model matrice, multiplied on the spot with glm. Note that the order is reversed,
	// As it seems glm reverses the way it calculates. (right hand multiplication)
	MatModel = glm::mat4();

	// translation
	MatModel = glm::translate(MatModel, Object->Transform.Position);

	// scaling
	MatModel = glm::scale(MatModel, Object->Transform.Scale);

	// added offset for recentering.
	MatModel = glm::translate(MatModel, glm::vec3(Object->BoundingBox.LocalCenter));
	// rotation
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.z), glm::vec3(0.0, 0.0, 1.0));

	// remove offset for recentering.
	MatModel = glm::translate(MatModel, glm::vec3(-Object->BoundingBox.LocalCenter));

	// Final MVP matrice merging.
	MatMVP = MatPerspectiveProjection * MatView * MatModel;

	// Send it to shader.
	GLint uniform_mat = glGetUniformLocation(MainShaderProgramme, "mvp_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatMVP[0][0]);
}


void		EngineController::applyMatricesToParticleObject(ParticleObject *Object)
{
	// Model matrice, multiplied on the spot with glm. Note that the order is reversed,
	// As it seems glm reverses the way it calculates. (right hand multiplication)
	MatModel = glm::mat4();

	// translation
	MatModel = glm::translate(MatModel, Object->Transform.Position);

	// scaling
	MatModel = glm::scale(MatModel, Object->Transform.Scale);

	// added offset for recentering.
	// MatModel = glm::translate(MatModel, glm::vec3(Object->BoundingBox.LocalCenter));
	// rotation
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.z), glm::vec3(0.0, 0.0, 1.0));

	// remove offset for recentering.
	// MatModel = glm::translate(MatModel, glm::vec3(-Object->BoundingBox.LocalCenter));

	// Final MVP matrice merging.
	MatMVP = MatPerspectiveProjection * MatView * MatModel;	

	// Send it to shader.
	GLint uniform_mat = glGetUniformLocation(MainShaderProgramme, "mvp_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatMVP[0][0]);
}