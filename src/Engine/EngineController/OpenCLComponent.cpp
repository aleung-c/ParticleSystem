#include "../../../includes/ParticleSystem.hpp"

OpenCLComponent::OpenCLComponent()
{
	CommandQueue = NULL;
	Program = NULL;
	source_str = NULL;
	nbKernels = 0;
}

OpenCLComponent::~OpenCLComponent()
{

}

/*
**	Initializing openCL context with OpenGL interoperability.
**	For this project, we will separate the OPENCL initialization.
**	-> this means this method must be called on its own after the
**	engine's first initialization.
*/

int				OpenCLComponent::InitOpenCL()
{
	int ret;

	ret = clGetPlatformIDs(1, &PlatformID, &RetNumPlatforms);
	ret = clGetDeviceIDs(PlatformID, CL_DEVICE_TYPE_GPU, 1, &DeviceID, &RetNumDevices);

	// create GL sharegroup for the open CL context.
	CGLContext = CGLGetCurrentContext();
	ShareGroup = CGLGetShareGroup(CGLContext);
	gcl_gl_set_sharegroup(ShareGroup);

#if defined(__APPLE__)
	// create he openCL context from the sharegroup. -> APPLE WAY.
	cl_context_properties props[] =
	{
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		(cl_context_properties) ShareGroup,
		CL_CONTEXT_PLATFORM,
		(cl_context_properties) PlatformID, 0
	};
	Context = clCreateContext(props, 1, &DeviceID, NULL, NULL, NULL);
#else
	cl_context_properties props[] =
	{
		CL_GL_CONTEXT_KHR,
		(cl_context_properties) wglGetCurrentContext(), // HGLRC handle
		CL_WGL_HDC_KHR,
		(cl_context_properties) wglGetCurrentDC(), // HDC handle
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)cpPlatform, 0
	};
	Context = clCreateContext(props, 1, &DeviceID, NULL, NULL, NULL);
#endif
	/* Create Command Queue */
	CommandQueue = clCreateCommandQueue(Context, DeviceID, 0, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("clCreateCommandQueue: %s\n", GetCLErrorString(ret));
	}
	return (0);
}

/*
**	Here, ill load the file with the kernel code.
*/

void			OpenCLComponent::LoadKernelFile(std::string pathName)
{
	/* Load the source code containing the kernel*/
	if (source_str)
		free(source_str);
	fp = fopen(pathName.c_str(), "r");
	if (!fp)
	{
		fprintf(stderr, "fopen: Failed to open kernel file.\n");
		exit (-1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
}

/*
**	Here, ill build the current program taken from LoadKernelFile
*/

void			OpenCLComponent::BuildProgram()
{
	/* Create Kernel Program from the source */
	Program = clCreateProgramWithSource(Context, 1,
		(const char **)&source_str,
		(const size_t *)&source_size, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("clCreateProgramWithSource: %s\n", GetCLErrorString(ret));
	}
	/* Build Kernel Program */
	ret = clBuildProgram(Program, 1, &DeviceID, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("Build CL Program: %s\n", GetCLErrorString(ret));
		// print error log for Program build (CLcompiler errors)
		if (ret == CL_BUILD_PROGRAM_FAILURE)
		{
			// Determine the size of the log
			size_t		log_size;
			clGetProgramBuildInfo(Program, DeviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			// Allocate and get memory for the log
			char		*log = (char *) malloc(log_size);
			clGetProgramBuildInfo(Program, DeviceID, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
			printf("%s\n", log);
		}
	}
}

/*
**	Then, ill create a kernel. There can be several kernels, so i made a slot system.
**	This method sends back the slot number of the kernel;
*/

int				OpenCLComponent::CreateKernel(std::string functionName)
{
	/* Create OpenCL Kernel */
	Kernels.push_back(clCreateKernel(Program, functionName.c_str(), &ret)); // <-- Function name in kernel !!
	nbKernels += 1;
	if (ret != CL_SUCCESS)
	{
		printf("Create CL kernel: %s\n", GetCLErrorString(ret));
		exit(-1);
	}
	return (nbKernels - 1);
}

void			OpenCLComponent::SetKernelArg(int kernelSlot, int arg_index, size_t size, void *ptr)
{
	if (kernelSlot >= nbKernels)
	{
		printf("Asked Kernel not created\n");
		return ;
	}
	ret = clSetKernelArg(Kernels[kernelSlot], arg_index, size, ptr);
	if (ret != CL_SUCCESS)
	{
		printf("Set CL kernel arg: %s\n", GetCLErrorString(ret));
		exit (-1);
	}
}

/*
**	After some test, I decided to let the api decides the localworksize,
**	because IF the localworksize IS NOT multiple of globalworksize,
**	nothing gets processed, and my number of particles is decided by user.
**
**	Also, im using NDRange for execution to have data parallelism, and not
**	task parallelism.
*/

void			OpenCLComponent::ExecuteParticleKernel(int kernelSlot, ParticleObject *particle)
{
	int		ret;
	size_t localWorkSize[] = {64, 16};
	size_t numLocalGroups[] = {ceil(particle->ParticleNumber/localWorkSize[0]),
								ceil(particle->ParticleNumber/localWorkSize[1])};
	size_t globalSize[] = {localWorkSize[0] * numLocalGroups[0], localWorkSize[1] * numLocalGroups[1]};

	glFinish();
	clEnqueueAcquireGLObjects(CommandQueue, 1, &particle->ObjMem, 0, 0, 0);
	if ((size_t)particle->ParticleNumber < localWorkSize[0])
	{
		globalSize[0] = (size_t)particle->ParticleNumber;
		localWorkSize[0] = (size_t)particle->ParticleNumber;
	}

	ret = clEnqueueNDRangeKernel(CommandQueue, Kernels[kernelSlot], 1, NULL, globalSize,
		localWorkSize, 0, 0, 0);
	if (ret != CL_SUCCESS)
	{
		std::cout << "clEnqueueNDRangeKernel error: " << GetCLErrorString(ret) << std::endl;
	}
	clEnqueueReleaseGLObjects(CommandQueue, 1, &particle->ObjMem, 0, 0, 0);
	clFinish(CommandQueue);
}

void			OpenCLComponent::CleanMemory()
{
	int		ret;

	ret = clFlush(CommandQueue);
	ret = clFinish(CommandQueue);
	for (std::vector<cl_kernel>::iterator it = Kernels.begin(); it != Kernels.end(); it++)
	{
		ret = clReleaseKernel(*it);
		if (ret != CL_SUCCESS)
			std::cout << "kernel release error: " << GetCLErrorString(ret) << std::endl;
	}
	ret = clReleaseProgram(Program);
	if (ret != CL_SUCCESS)
			std::cout << "program release error: " << GetCLErrorString(ret) << std::endl;
	ret = clReleaseCommandQueue(CommandQueue);
	if (ret != CL_SUCCESS)
			std::cout << "cmd queue release error: " << GetCLErrorString(ret) << std::endl;
	ret = clReleaseContext(Context);
	if (ret != CL_SUCCESS)
			std::cout << "context release error: " << GetCLErrorString(ret) << std::endl;
	if (source_str)
		free(source_str);
	std::cout << "OpenCL memory freed!" << std::endl;
}

const char		*OpenCLComponent::GetCLErrorString(cl_int error)
{
	switch(error)
	{
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
