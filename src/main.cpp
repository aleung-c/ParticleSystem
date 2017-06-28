#include "../includes/ParticleSystem.hpp"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int		main(void)
{
	EngineController *Engine = &EngineController::Instance();

	std::cout << "Hello particle system!" << std::endl;
	Engine->InitEngine(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

	ParticleObject *particle = new ParticleObject(1000);

	// GameObject *test_obj = new GameObject("test_obj", "./ressources/chair_1.obj");

	if (particle)
	{}

	cl_int					ret;
	cl_command_queue		command_queue = NULL;

	FILE					*fp;
	char					fileName[] = "./kernels/particles_position.cl";
	char					*source_str;
	size_t					source_size;

	cl_kernel				kernel = NULL;
	cl_program				program = NULL;


	// /* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp)
	{
		fprintf(stderr, "fopen: Failed to open kernel file.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(Engine->Context, Engine->DeviceID, 0, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("clCreateCommandQueue: %s\n", getCLErrorString(ret));
	}

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(Engine->Context, 1,
		(const char **)&source_str,
		(const size_t *)&source_size, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("clCreateProgramWithSource: %s\n", getCLErrorString(ret));
	}

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &Engine->DeviceID, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("Build CL program: %s\n", getCLErrorString(ret));
		// print error log for program build (CLcompiler errors)
		if (ret == CL_BUILD_PROGRAM_FAILURE)
		{
			// Determine the size of the log
			size_t log_size;
			clGetProgramBuildInfo(program, Engine->DeviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
			// Allocate memory for the log
			char *log = (char *) malloc(log_size);
			// Get the log
			clGetProgramBuildInfo(program, Engine->DeviceID, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
			printf("%s\n", log);
		}
	}

	

	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "place_particles", &ret); // <-- Function name in kernel !!
	if (ret != CL_SUCCESS)
	{
		printf("Create CL kernel: %s\n", getCLErrorString(ret));
	}


	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&particle->ObjMem);
	if (ret != CL_SUCCESS)
	{
		printf("Set CL kernel arg: %s\n", getCLErrorString(ret));
	}

	/* Execute OpenCL Kernel */
	// ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	// ret = clEnqueueReadBuffer(command_queue, particle->ObjMem, CL_TRUE, 0,
	// MEM_SIZE * sizeof(char), string, 0, NULL, NULL);

	// /* Display Result */
	// puts(string);

	Engine->CheckForOpenGLErrors();
	size_t	globalWorkSize;
	size_t	localWorkSize;

	globalWorkSize = particle->ParticleNumber;
	localWorkSize = 1;
	while (!glfwWindowShouldClose(Engine->Window))
	{
		glFinish();
		clEnqueueAcquireGLObjects(command_queue, 1, &particle->ObjMem, 0, 0, 0);
		// ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&particle->ObjMem);
		if (ret != CL_SUCCESS)
		{
			printf("Set CL kernel arg: %s\n", getCLErrorString(ret));
		}
		clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &globalWorkSize,
			&localWorkSize, 0, 0, 0);
		clEnqueueReleaseGLObjects(command_queue, 1, &particle->ObjMem, 0, 0, 0);
		clFinish(command_queue);
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

const char *getCLErrorString(cl_int error)
{
switch(error){
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}
