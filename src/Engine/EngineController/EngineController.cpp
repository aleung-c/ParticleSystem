#include "../../../includes/ParticleSystem.hpp"

// --------------------------------------------------------------------------------	//
//																					//
//	GAME ENGINE CONTROLLER															//
//																					//
//	This class is the core of the engine.											//
//																					//
//	It handles every core components of the engine.									//
//	This means it init the GLFW window, the truetype library,						//
//	and the main drawing loop and collected GameObject processing					//
//	and displaying																	//
//																					//
// -------------------------------------------------------------------------------	//

/*
**	This is the engine's singleton instance. With this line, linking the engine
**	through compilation will create the factory style object's creator.
**	This line allows you to make var = new GameObject() and not have to consider the whole
**	opengl loading/drawing.
*/

// ******************** SINGLETON PATTERN *************************************** //

EngineController EngineController::m_instance = EngineController();

// ******************** SINGLETON PATTERN *************************************** //

// default constructor
EngineController::EngineController() :
	DebugMode(false),
	EngineInitialized(false)
{
	// Set engine default Settings;
	Settings.TextureMipMapValue = TEXTUREMIPMAP_DEFAULT;
	Settings.AntiAliasingValue = ANTIALIASING_DEFAULT;
}

// default destructor
EngineController::~EngineController()
{

}

EngineController& EngineController::Instance()
{
	return m_instance;
}

// --------------------------------------------------------------------	//
//																		//
//	Engine inits														//
//																		//
// --------------------------------------------------------------------	//

/*
**	This method MUST be called through the game code, as it will set the 
**	window and engine's variables. Your program will certainly segfault
**	if this method is not called.
*/

void		EngineController::InitEngine(int windowWidth, int windowHeight, std::string windowName)
{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;
	WindowName = windowName;
	if (initGLFW() == -1
		|| initOpenGL() == -1
		|| initFreeType() == -1
		|| initOpenCL())
	{
		std::cout << "Fatal error: Initialization error. Exiting..." << std::endl;
		exit (-1);
	}
	loadShaders();
	loadMatrices();
	EngineInitialized = true;
	if (DebugMode)
	{
		CheckForOpenGLErrors();
		std::cout << "EngineController: Engine initialized!" << std::endl;
	}
}

/*
**	Debug method for opengl related problems.
*/

void	EngineController::CheckForOpenGLErrors()
{
	int error = 0;

	// check if main 3d perspective shader is compiled and linked;
	GLint isLinked = 0;
	glGetProgramiv(MainShaderProgramme, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		printf(KRED "Error: Main Shader programme NOT linked%s\n", KRESET);
		error = -1;
	}
	else
	{
		if (DebugMode)
		{
			printf(KGRN "Main Shader programme linked%s\n", KRESET);
		}
	}
	// check if ortho shader is compiled and linked;
	isLinked = 0;
	glGetProgramiv(OrthoShaderProgramme, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		printf(KRED "Error: Text Shader programme NOT linked%s\n", KRESET);
		error = -1;
	}
	else
	{
		if (DebugMode)
		{
			printf(KGRN "Text Shader programme linked%s\n", KRESET);
		}
	}
	if (error == 0)
		return ;
	// check for opengl errors.
	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf(KRED "OpenGL Error: %u%s\n", err, KRESET);
	exit (-1);
}

// --------------------------------------------------------------------	//
//																		//
//	Library inits														//
//																		//
// --------------------------------------------------------------------	//

/*
**	The GLFW initialization handles the window and the openGL context.
**	As this engine is made for small not good looking games, ill only load
**	ONE context, and as such, ONE screen. For now, the resolution changing
**	during runtime is not supported.
*/

int		EngineController::initGLFW()
{
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit ())
	{
		std::cout << "ERROR: could not start GLFW3" << std::endl;
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ----- AntiAliasing
	glfwWindowHint(GLFW_SAMPLES, Settings.AntiAliasingValue);

	Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName.c_str(), NULL, NULL);
	if (!Window)
	{
		std::cout << "ERROR: could not open window with GLFW3" << std::endl;
		glfwTerminate();
		return (-1);
	}
	// Create context
	glfwMakeContextCurrent (Window);
	return (0);
}

int		EngineController::initOpenGL()
{
	if (DebugMode)
	{
		// get version info
		const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
		const GLubyte* version = glGetString (GL_VERSION); // version as a string
		std::cout << "Renderer: " << renderer << std::endl;
		std::cout << "OpenGL version supported " << version << std::endl;
	}

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glEnable(GL_POLYGON_OFFSET_FILL);
	// glPolygonOffset(1, 0);
	

	// for antialiasing.
	glEnable(GL_MULTISAMPLE);

	// glEnable(GL_ALPHA_TEST);

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif
	return (0);
}

/*
**	Initializing openCL context through GLFW with a special known technique.
*/

int		EngineController::initOpenCL()
{
	int ret;

	ret = clGetPlatformIDs(1, &PlatformID, &RetNumPlatforms);
	ret = clGetDeviceIDs(PlatformID, CL_DEVICE_TYPE_GPU, 1, &DeviceID, &RetNumDevices);

	CGLContext = CGLGetCurrentContext();
	ShareGroup = CGLGetShareGroup(CGLContext);
	gcl_gl_set_sharegroup(ShareGroup);

	return (0);
}

// --------------------------------------------------------------------	//
//																		//
//	Engine side drawing													//
//	To see each object rendering in detail,								//
//	look at EngineController_rendering.cpp							//
//																		//
// --------------------------------------------------------------------	//

/*
**	Main public drawing function. This will be called at each loop turn,
**	it MUST be used in the game's main loop.
*/

void	EngineController::Draw()
{
	if (EngineInitialized)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw3DModels();
		drawParticleObjects();
		drawUIObjects();
		glClear(GL_DEPTH_BUFFER_BIT);
		drawTextObjects();


		// display on screen.
		glfwSwapBuffers(Window);
	}
	else
	{
		std::cout << "Error: Engine not initialized! Use InitEngine() first." << std::endl;
		exit(-1);
	}
}

/*
**	Run through and display each GameObject that has a model/texture.
*/

void	EngineController::draw3DModels()
{
	// glUseProgram(MainShaderProgramme);

	// Reset Camera VIEW matrix for camera movement.
	MatView = glm::lookAt(MainCamera->Transform.Position, CameraLookAt, glm::vec3(0.0, 2.0, 0.0));

	// draw 3d objects
	// run through each object to set their matrices and textures and draw them on screen.
	for (std::vector<GameObject *>::iterator it = GameObjectList.begin();
		it != GameObjectList.end();
		it++)
	{
		if ((*it)->Visible == true)
		{
			if ((*it)->MorphAnimation.IsAnimated() == true)
			{
				glUseProgram(MorphTargetProgramme);
				renderMorphAnimation(*it);
			}
			else
			{
				glUseProgram(MainShaderProgramme);
				render3DGameObject(*it);
			}
			if (DebugMode == true)
			{
				glUseProgram(MainShaderProgramme);
				renderBoundingBox(*it);
			}
		}
	}
}

/*
**	Run through and display each GameTextObject and render the text.
**	See EngineController_freetype_font.cpp
*/

void	EngineController::drawUIObjects()
{
	glUseProgram(OrthoShaderProgramme);
	// set projection matrice.
	GLint uniform_mat = glGetUniformLocation(OrthoShaderProgramme, "projection_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatOrthographicProjection[0][0]);
	glUniform1i(glGetUniformLocation(OrthoShaderProgramme, "IsText"), GL_FALSE);
	for (std::vector<GameUIObject *>::iterator it = GameUIObjectList.begin();
		it != GameUIObjectList.end();
		it++)
	{
		if ((*it)->Visible == true)
		{
			renderGameUIObject(*it);
		}
	}
}

/*
**	Run through and display each GameUIObject and render them.
**	See EngineController_UI.cpp for rendering. Uses mostly the same
**	rendering method as the textObject, but has only one quad to draw.
*/

void	EngineController::drawTextObjects()
{
	glUseProgram(OrthoShaderProgramme);
	// set projection matrice.
	GLint uniform_mat = glGetUniformLocation(OrthoShaderProgramme, "projection_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatOrthographicProjection[0][0]);
	glUniform1i(glGetUniformLocation(OrthoShaderProgramme, "IsText"), GL_TRUE);
	for (std::vector<GameTextObject *>::iterator it = GameTextObjectList.begin();
		it != GameTextObjectList.end();
		it++)
	{
		if ((*it)->Visible == true)
		{
			renderGameTextObject(*it);
		}
	}
}

void	EngineController::drawParticleObjects()
{
	glUseProgram(MainShaderProgramme);
	for (std::vector<ParticleObject *>::iterator it = ParticleObjectList.begin();
		it != ParticleObjectList.end();
		it++)
	{
		if ((*it)->Visible == true)
		{
			renderParticleObject(*it);
		}
	}
}
