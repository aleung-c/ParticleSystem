#ifndef OPENCLCOMPONENT_HPP
# define OPENCLCOMPONENT_HPP

class OpenCLComponent
{
	public:
		OpenCLComponent();
		~OpenCLComponent();

		// gl cl sharing.
		cl_context					Context;
		CGLContextObj				CGLContext;
		CGLShareGroupObj			ShareGroup;

		// cl default variables.
		cl_platform_id				PlatformID;
		cl_device_id				DeviceID;
		cl_command_queue			CommandQueue;
		cl_program					Program;

		cl_uint						RetNumDevices;
		cl_uint						RetNumPlatforms;

		cl_kernel					Kernel;

		int							InitOpenCL();
		void						LoadKernel(std::string pathName);
		void						BuildProgram(std::string functionName);
		void						SetKernelArg(int arg_index, size_t size, void *ptr);
		void						ExecuteParticleKernel(ParticleObject *particle);
		const char					*GetCLErrorString(cl_int error);

	private:
		cl_int						ret;

		FILE						*fp;
		char						*source_str;
		size_t						source_size;
};

#endif