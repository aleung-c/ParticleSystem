#include "../../../includes/ParticleSystem.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Shaders inits														//
//																		//
// --------------------------------------------------------------------	//

/*
**	I made the engine with two simples shaders,
**	one for the 3d objects with a perspective representation,
**	and another for the orthographically represented objects,
**	that are the text objects, generated through freetype,
**	and the UI textured components.
*/

void	EngineController::loadShaders()
{
	// -------------------------------------------------------------------------- //
	//	3d model Shaders -> main shader											  //
	// -------------------------------------------------------------------------- //
	// Go get Position shader
	VertexShader_1 = GetFileContent("./shaders/vshader_1.vs");
	FragmentShader_1 = GetFileContent("./shaders/fshader_1.fs");

	// Create shader programme
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const char * const *)&VertexShader_1, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const char * const *)&FragmentShader_1, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram ();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram (shader_programme);
	MainShaderProgramme = shader_programme;
	free(VertexShader_1);

	// -------------------------------------------------------------------------- //
	//	Orthographic Shaders -> UI and text										  //
	// -------------------------------------------------------------------------- //
	TextVShader = GetFileContent("./shaders/ortho_vshader_1.vs");
	TextFShader = GetFileContent("./shaders/ortho_fshader_1.fs");

	// Create shader programme
	GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs2, 1, (const char * const *)&TextVShader, NULL);
	glCompileShader(vs2);
	GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs2, 1, (const char * const *)&TextFShader, NULL);
	glCompileShader(fs2);

	GLuint shader_programme2 = glCreateProgram ();
	glAttachShader(shader_programme2, fs2);
	glAttachShader(shader_programme2, vs2);
	glLinkProgram (shader_programme2);
	OrthoShaderProgramme = shader_programme2;

	// ------------------------------------------------------------------------ //
	//	Morph Target animation Shaders											//
	// ------------------------------------------------------------------------ //

	VertexShader_1 = GetFileContent("./shaders/morph_target_vshader.vs");
	// FragmentShader_1 = GetFileContent("./shaders/fshader_1.fs");

	// Create shader programme
	GLuint vs3 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs3, 1, (const char * const *)&VertexShader_1, NULL);
	glCompileShader(vs3);
	GLuint fs3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs3, 1, (const char * const *)&FragmentShader_1, NULL);
	glCompileShader(fs3);

	GLuint shader_programme3 = glCreateProgram ();
	glAttachShader(shader_programme3, fs3);
	glAttachShader(shader_programme3, vs3);
	glLinkProgram (shader_programme3);
	MorphTargetProgramme = shader_programme3;
}
