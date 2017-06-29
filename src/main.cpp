#include "../includes/ParticleSystem.hpp"



int		main(void)
{
	EngineController *Engine = &EngineController::Instance();

	std::cout << "Hello particle system!" << std::endl;
	Engine->InitEngine(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
	Engine->GetCamera()->Transform.Position.x = 0.0;
	Engine->GetCamera()->Transform.Position.y = 0.0;
	Engine->GetCamera()->Transform.Position.z = -5.0;
	Engine->SetCameraLookAt(glm::vec3(0.0, 0.0, 0.0));

	// Initializing OPENCL COMPONENT.
	Engine->CLController.InitOpenCL();
	Engine->CLController.LoadKernel("./kernels/particles_position.cl");
	Engine->CLController.BuildProgram("place_particles");

	// Initializing Scene.
	ParticleObject *particle = new ParticleObject(1000);
	particle->Transform.Position.x = 0.0;
	particle->Transform.Position.y = 0.0;
	particle->Transform.Position.z = 0.0;

	/* Set OpenCL Kernel Parameters */
	Engine->CLController.SetKernelArg(0, sizeof(cl_mem), (void *)&particle->ObjMem);
	Engine->CLController.SetKernelArg(1, sizeof(float *), (void *)&particle->Transform.Position);

	/* Execute the kernel, as of now in NDRange -> data parallelism. */
	Engine->CLController.ExecuteParticleKernel(particle);

	Engine->CheckForOpenGLErrors();
	while (!glfwWindowShouldClose(Engine->Window))
	{
		glfwPollEvents();
		Engine->Draw();
	}

	// ----- OpenCL hello world -----
	// cl_device_id			device_id = NULL;
	// cl_context				context = NULL;
	// cl_command_queue		command_queue = NULL;
	// cl_mem					memobj = NULL;
	// cl_program				program = NULL;
	// cl_kernel				kernel = NULL;
	// cl_platform_id			platform_id = NULL;
	// cl_uint					ret_num_devices;
	// cl_uint					ret_num_platforms;
	// cl_int					ret;

	// char					string[MEM_SIZE];

	// FILE					*fp;
	// char					fileName[] = "./kernels/hello.cl";
	// char					*source_str;
	// size_t					source_size;

	// /* Load the source code containing the kernel*/
	// fp = fopen(fileName, "r");
	// if (!fp)
	// {
	// 	fprintf(stderr, "fopen: Failed to open kernel file.\n");
	// 	exit(1);
	// }
	// source_str = (char *)malloc(MAX_SOURCE_SIZE);
	// source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	// fclose(fp);

	// /* Get Platform and Device Info */
	// ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	// ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	// /* Create OpenCL context */
	// context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// /* Create Command Queue */
	// command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// /* Create Memory Buffer */
	// memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, &ret);

	// /* Create Kernel Program from the source */
	// program = clCreateProgramWithSource(context, 1,
	// 	(const char **)&source_str,
	// 	(const size_t *)&source_size, &ret);

	// /* Build Kernel Program */
	// ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// /* Create OpenCL Kernel */
	// kernel = clCreateKernel(program, "hello", &ret); // <-- Function name in kernel !!

	// /* Set OpenCL Kernel Parameters */
	// ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

	// /* Execute OpenCL Kernel */
	// ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);

	// /* Copy results from the memory buffer */
	// ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
	// MEM_SIZE * sizeof(char), string, 0, NULL, NULL);

	// /* Display Result */
	// puts(string);

	// /* Finalization */
	// ret = clFlush(command_queue);
	// ret = clFinish(command_queue);
	// ret = clReleaseKernel(kernel);
	// ret = clReleaseProgram(program);
	// ret = clReleaseMemObject(memobj);
	// ret = clReleaseCommandQueue(command_queue);
	// ret = clReleaseContext(context);
	 
	// free(source_str);


	return 0;
}


