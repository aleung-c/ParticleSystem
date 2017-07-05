#ifndef PARTICLESYSTEM_HPP
# define PARTICLESYSTEM_HPP

// color in text;
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KRESET "\x1B[0m"

// General includes
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <iterator>
# include <cstdlib>
# include <iostream>
# include <fstream>
# include <sstream>
# include <fcntl.h>
# include <list>
# include <vector>
# include <string>
# include <queue>
# include <random>
# include <algorithm>
# include <math.h>
# include <map>

// opengl includes
# include "X.h"
# include <ctype.h>
# include <regex.h>

// lib GML - Opengl mathematics
# include "../glm/glm/glm.hpp"
# include "../glm/glm/gtc/matrix_transform.hpp"
# include "../glm/glm/gtx/rotate_vector.hpp"

// linking GLFW
# include "../glfw-3.2.1/include/GLFW/glfw3.h"

// Linking OpenGL -> for MacOS, untested elsewhere.
# if defined(__APPLE__)
#  define __gl_h_
#  define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#  include <OpenGL/gl3.h>
#  include <OpenGL/gl3ext.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
# else
#  include <GL/gl3.h>

// #  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glew.h>
# endif

// Linking OpenCL
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
#  include <cl.h>
# endif

# include <OpenCL/cl_gl.h>
# include <OpenCL/cl_gl_ext.h>
# include <OpenGL/CGLDevice.h>
# include <OpenGL/CGLCurrent.h>

# ifdef _WIN32
#  include <windows.h>
# endif

# define CL_GL_CONTEXT_KHR 0x2008
# define CL_WGL_HDC_KHR 0x200B

// Freetype - font library
# include <ft2build.h>
# include FT_FREETYPE_H

/*
**	Engine default values
*/

# define TEXTUREMIPMAP_DEFAULT 2
# define ANTIALIASING_DEFAULT 2
# define WINDOW_WIDTH 1280
# define WINDOW_HEIGHT 720
# define WINDOW_NAME "aleung-c's Particle System"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE 50000

/*
**	Particles defines
*/

# define MAX_NB_PARTICLES 3000000

/*
**	Objects type handled by the engine.
*/

enum EngineObjectType
{
	GAMEOBJECT,
	GAMEUIOBJECT,
	GAMETEXTOBJECT
};

/*
**	BMP helper struct
*/

typedef struct						s_bmp_texture
{
	// Data read from the header of the BMP file
	GLuint							texture_id;
	std::string						texture_path;
	unsigned char					header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int					data_pos;     // Position in the file where the actual data begins
	unsigned int					width;
	unsigned int					height;
	unsigned int					image_size;   // = width*height*3
	// Actual RGB data
	unsigned char					*data;
}									t_bmp_texture;

/*
**	Freetype character helper struct.
**	Thanks to Joey de Vries from https://learnopengl.com/#!In-Practice/Text-Rendering
*/

struct Character
{
	GLuint				TextureID;  // ID handle of the glyph texture
	glm::ivec2			Size;       // Size of glyph
	glm::ivec2			Bearing;    // Offset from baseline to left/top of glyph
	GLuint				Advance;    // Offset to advance to next glyph
};



/*
**	Class forward declarations
*/

// Engine mother Object class;
class					EngineObject;

// Objects' components
class					TransformComponent;
class					BoundingBoxComponent;
class					TextureComponent;


// Main objects
class					GameObject;
class					GameTextObject;
class					GameUIObject;
class					ParticleObject;

// Main Controller
class					TextureManager;
class					EngineSettings;
class					OpenCLComponent;

class					EngineController;

typedef struct			s_ParticleSystemDatas
{
	int					NbParticlesAsked;

	EngineController	*engine;
	ParticleObject		*Particle;

	GLuint				Randsuite_vao;
	GLuint				Randsuite_vbo;
	cl_mem				Randsuite_ObjMem;

	float				GravityPoint_x;
	float				GravityPoint_y;

	double				Cursor_x;
	double				Cursor_y;

	float				World_fCursor_x;
	float				World_fCursor_y;

	bool				FollowKeyPressed;
	// float				Cursor_pos[4];

}						t_ParticleSystemDatas;

// file includes - order matters.

# include "ObjectsComponents/TransformComponent.hpp"

# include "ObjectsComponents/TextureComponent.hpp"

# include "EngineObjects/EngineObject.hpp"

# include "ObjectsComponents/BoundingBoxComponent.hpp"

# include "ObjectsComponents/MorphTargetAnimation.hpp"

# include "EngineObjects/GameObject.hpp"
# include "EngineObjects/GameTextObject.hpp"
# include "EngineObjects/GameUIObject.hpp"
# include "EngineObjects/ParticleObject.hpp"

# include "TextureManager.hpp"
# include "EngineSettings.hpp"

# include "OpenCLComponent.hpp"
# include "EngineController.hpp"

/*
**	C style function prototypes.
*/

void					ParticleSystem_GetArgs(int argc, char **argv, t_ParticleSystemDatas *PSDatas);

void					ParticleSystem_Init(EngineController *engine, t_ParticleSystemDatas *PSDatas);
void					ParticleSystem_SceneInit(EngineController *engine, t_ParticleSystemDatas *PSDatas);
void					PositionParticlesRandomly(EngineController *engine, t_ParticleSystemDatas *PSDatas);

void					PrepareParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas);
void					UpdateParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas);

void					ParticleSystem_MainLoop(EngineController *engine, t_ParticleSystemDatas *PSDatas);

void					ParticleSystem_MemoryClean(EngineController *engine, t_ParticleSystemDatas *PSDatas);

double					GetRandomDouble(double min, double max);

void					KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void					GetMouseWorldPosition(t_ParticleSystemDatas *PSDatas);

# endif
