#ifndef EngineCONTROLLER_HPP
# define EngineCONTROLLER_HPP

# include "ParticleSystem.hpp"

/*
**	This engine is a GameObject collector. When a new GameObject
**	is created, it goes into the engine's list of object,
**	and this engine will draw it automatically, applying matrices
**	and stuff.
*/

class	EngineController
{
	public:
		// ----- singleton -----
		static EngineController&		Instance();
		// ----- singleton -----

		bool								DebugMode;

		EngineSettings						Settings;

		int									WindowWidth;
		int									WindowHeight;
		std::string							WindowName;
		GLFWwindow							*Window;
		
		bool								EngineInitialized;

		// ------ Engine's objects handling ------
		std::vector<GameObject *>			GameObjectList;
		std::vector<GameTextObject *>		GameTextObjectList;
		std::vector<GameUIObject *>			GameUIObjectList;
		std::vector<ParticleObject *>		ParticleObjectList;

		// ------ Engine's textures handling ------
		TextureManager						TextureManager;

		// ------ Engine's Camera handling ------
		GameObject							*MainCamera;
		glm::vec3							CameraLookAt;

		// ------ Shaders handling ------
		char								*VertexShader_1;
		char								*FragmentShader_1;

		char								*TextVShader;
		char								*TextFShader;

		GLuint								MainShaderProgramme;
		GLuint								OrthoShaderProgramme;
		GLuint								MorphTargetProgramme;

		// ------ Matrix handling ------
		// Model
		glm::mat4							MatModelIdentity;
		glm::mat4							MatModelTranslation;
		glm::mat4							MatModelRecenter;
		glm::mat4							MatModelRecenterInverted;
		glm::mat4							MatModelRotation;
		glm::mat4							MatModelScaling;

		// View -> camera
		float								CameraNear;
		float								CameraFar;
		float								CameraFov;
		float								CameraAspect;

		// Projection -> persp or ortho
		glm::mat4							MatPerspectiveProjection;
		glm::mat4							MatOrthographicProjection;

		// Matrices merge
		glm::mat4							MatModel;
		glm::mat4							MatView;

		glm::mat4							MatMVP;

		// ------ Freetype lib fonts ------
		FT_Library							FT_Lib;
		FT_Face								Face;

		// map of asciis 128 characters.
		std::map<GLchar, Character>			Characters;

		// ----- OpenCL variables
		// gl cl sharing.
		CGLContextObj						CGLContext;
		CGLShareGroupObj					ShareGroup;

		// cl default variables.
		cl_platform_id						PlatformID;
		cl_device_id						DeviceID;
		// cl_context							Context;
		cl_command_queue					CommandQueue;
		cl_program							Program;
		cl_kernel							Kernel;

		cl_uint								RetNumDevices;
		cl_uint								RetNumPlatforms;

		// ----- Public Engine's methods.
		EngineController();
		~EngineController();

		void								InitEngine(int windowWidth, int windowHeight, std::string WindowName);
		void								CheckForOpenGLErrors();
		void								Draw();

		static int							LoadTextureFile(t_bmp_texture *texture, std::string path);
		static char							*GetFileContent(std::string file_path);

		GameObject							*GetCamera();
		glm::vec3							*GetCameraLookAt();
		void								SetCamera(GameObject *NewCamera);
		void								SetCameraLookAt(glm::vec3 new_look_pos);

	private:
		// ----- Initialization methods.
			
		int									initGLFW();
		int									initOpenGL();
		int									initOpenCL();
		int									initFreeType();
		void								loadFreeTypesCharacters();

		// ----- Shaders.
		void								loadShaders();
		
		// ----- Matrice handling.

		glm::vec4							_tmpVec;

		void								loadMatrices();
		void								applyMatricesToObject(GameObject *Object);
		void								applyMatricesToParticleObject(ParticleObject *Object);

		// ----- Textures.
		void								loadObjectTexture(GameObject *Object);


		// ----- Object drawing.
		void								draw3DModels();
		void								drawUIObjects();
		void								drawTextObjects();
		void								drawParticleObjects();

		// ----- Object rendering.
		void								render3DGameObject(GameObject *obj);
		void								renderMorphAnimation(GameObject *obj);
		void								renderBoundingBox(GameObject *obj);
		void								renderGameUIObject(GameUIObject *obj);
		void								renderGameTextObject(GameTextObject *obj);
		void								renderParticleObject(ParticleObject *obj);

		static EngineController			m_instance;

		// ------ Font tmp vars ------
		int									tmp_x;
		int									tmp_y;
		Character							ch;
		GLfloat								xpos;
		GLfloat								ypos;
		GLfloat								w;
		GLfloat								h;
};

#endif
