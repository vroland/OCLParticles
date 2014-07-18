/*
 * CLHandler.cpp
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#include "CLHandler.h"
#include "utils.h"
#include "constants.h"

#include <iostream>

using namespace std;

void clPrintDevInfo(cl::Device device) {
	string device_string;

// CL_DEVICE_NAME
	device_string = device.getInfo<CL_DEVICE_NAME>();
	printf(" CL_DEVICE_NAME: \t\t\t%s\n", device_string.c_str());

// CL_DEVICE_VENDOR
	device_string = device.getInfo<CL_DEVICE_VENDOR>();
	printf(" CL_DEVICE_VENDOR: \t\t\t%s\n", device_string.c_str());

// CL_DRIVER_VERSION
	device_string = device.getInfo<CL_DRIVER_VERSION>();
	printf(" CL_DRIVER_VERSION: \t\t\t%s\n", device_string.c_str());

// CL_DEVICE_INFO
	cl_int type = device.getInfo<CL_DEVICE_TYPE>();
	if (type & CL_DEVICE_TYPE_CPU)
		printf(" CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_CPU");
	if (type & CL_DEVICE_TYPE_GPU)
		printf(" CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_GPU");
	if (type & CL_DEVICE_TYPE_ACCELERATOR)
		printf(" CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_ACCELERATOR");
	if (type & CL_DEVICE_TYPE_DEFAULT)
		printf(" CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_DEFAULT");

// CL_DEVICE_MAX_COMPUTE_UNITS
	cl_uint compute_units = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
	printf(" CL_DEVICE_MAX_COMPUTE_UNITS:\t\t%u\n", compute_units);

// CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
	size_t workitem_dims = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
	printf(" CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:\t%u\n", workitem_dims);

// CL_DEVICE_MAX_WORK_ITEM_SIZES
	vector<size_t> workitem_size = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();

	printf(" CL_DEVICE_MAX_WORK_ITEM_SIZES:\t%u / %u / %u \n", workitem_size[0],
			workitem_size[1], workitem_size[2]);

// CL_DEVICE_MAX_WORK_GROUP_SIZE
	size_t workgroup_size = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
	printf(" CL_DEVICE_MAX_WORK_GROUP_SIZE:\t%u\n", workgroup_size);

// CL_DEVICE_MAX_CLOCK_FREQUENCY
	cl_uint clock_frequency = device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();
	printf(" CL_DEVICE_MAX_CLOCK_FREQUENCY:\t%u MHz\n", clock_frequency);

// CL_DEVICE_ADDRESS_BITS
	cl_uint addr_bits = device.getInfo<CL_DEVICE_ADDRESS_BITS>();
	printf(" CL_DEVICE_ADDRESS_BITS:\t\t%u\n", addr_bits);

// CL_DEVICE_MAX_MEM_ALLOC_SIZE
	cl_ulong max_mem_alloc_size = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();
	printf(" CL_DEVICE_MAX_MEM_ALLOC_SIZE:\t\t%u MByte\n",
			(unsigned int) (max_mem_alloc_size / (1024 * 1024)));

// CL_DEVICE_GLOBAL_MEM_SIZE
	cl_ulong mem_size = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
	printf(" CL_DEVICE_GLOBAL_MEM_SIZE:\t\t%u MByte\n",
			(unsigned int) (mem_size / (1024 * 1024)));

// CL_DEVICE_ERROR_CORRECTION_SUPPORT
	cl_bool error_correction_support = device.getInfo<CL_DEVICE_ERROR_CORRECTION_SUPPORT>();
	printf(" CL_DEVICE_ERROR_CORRECTION_SUPPORT:\t%s\n",
			error_correction_support == CL_TRUE ? "yes" : "no");

// CL_DEVICE_LOCAL_MEM_TYPE
	cl_device_local_mem_type local_mem_type = device.getInfo<CL_DEVICE_LOCAL_MEM_TYPE>();
	printf(" CL_DEVICE_LOCAL_MEM_TYPE:\t\t%s\n",
			local_mem_type == 1 ? "local" : "global");

// CL_DEVICE_LOCAL_MEM_SIZE
	mem_size  = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
	printf(" CL_DEVICE_LOCAL_MEM_SIZE:\t\t%u KByte\n",
			(unsigned int) (mem_size / 1024));

// CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
	mem_size  = device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();
	printf(" CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:\t%u KByte\n",
			(unsigned int) (mem_size / 1024));

// CL_DEVICE_QUEUE_PROPERTIES
	cl_command_queue_properties queue_properties = device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();
	if (queue_properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
		printf(" CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n",
				"CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE");
	if (queue_properties & CL_QUEUE_PROFILING_ENABLE)
		printf(" CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n",
				"CL_QUEUE_PROFILING_ENABLE");

// CL_DEVICE_IMAGE_SUPPORT
	cl_bool image_support = device.getInfo<CL_DEVICE_IMAGE_SUPPORT>();
	printf(" CL_DEVICE_IMAGE_SUPPORT:\t\t%u\n", image_support);

// CL_DEVICE_MAX_READ_IMAGE_ARGS
	cl_uint max_read_image_args = device.getInfo<CL_DEVICE_MAX_READ_IMAGE_ARGS>();
	printf(" CL_DEVICE_MAX_READ_IMAGE_ARGS:\t%u\n", max_read_image_args);

// CL_DEVICE_MAX_WRITE_IMAGE_ARGS
	cl_uint max_write_image_args = device.getInfo<CL_DEVICE_MAX_WRITE_IMAGE_ARGS>();
	printf(" CL_DEVICE_MAX_WRITE_IMAGE_ARGS:\t%u\n", max_write_image_args);

// CL_DEVICE_IMAGE2D_MAX_WIDTH, CL_DEVICE_IMAGE2D_MAX_HEIGHT, CL_DEVICE_IMAGE3D_MAX_WIDTH, CL_DEVICE_IMAGE3D_MAX_HEIGHT, CL_DEVICE_IMAGE3D_MAX_DEPTH
	size_t szMaxDims[5] = {
			device.getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH>(),
			device.getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT>(),
			device.getInfo<CL_DEVICE_IMAGE3D_MAX_WIDTH>(),
			device.getInfo<CL_DEVICE_IMAGE3D_MAX_HEIGHT>(),
			device.getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH>()
	};
	printf("\n CL_DEVICE_IMAGE <dim>");
	printf("\t\t\t2D_MAX_WIDTH\t %u\n", szMaxDims[0]);
	printf("\t\t\t\t\t2D_MAX_HEIGHT\t %u\n", szMaxDims[1]);
	printf("\t\t\t\t\t3D_MAX_WIDTH\t %u\n", szMaxDims[2]);
	printf("\t\t\t\t\t3D_MAX_HEIGHT\t %u\n", szMaxDims[3]);
	printf("\t\t\t\t\t3D_MAX_DEPTH\t %u\n", szMaxDims[4]);

// CL_DEVICE_PREFERRED_VECTOR_WIDTH_<type>
	printf(" CL_DEVICE_PREFERRED_VECTOR_WIDTH_<t>\t");
	cl_uint vec_width[6] =  {
			device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR>(),
			device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT>(),
			device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT>(),
			device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG>(),
			device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT>(),
			device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE>(),
	};

	printf("CHAR %u, SHORT %u, INT %u, FLOAT %u, DOUBLE %u\n\n\n", vec_width[0],
			vec_width[1], vec_width[2], vec_width[3], vec_width[4]);
}

CLHandler::CLHandler() {

	glFinish();

	valid = true;

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	if (platforms.size() == 0) {
		std::cout << "Platform size 0\n";
		valid = false;
		return;
	}

	cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties) (cl_context_properties)(platforms[0])(),
									  CL_GL_CONTEXT_KHR, (intptr_t) SDL_GL_GetCurrentContext(), 0 };

	context = cl::Context(CL_DEVICE_TYPE_GPU, properties);
	devices = context.getInfo<CL_CONTEXT_DEVICES>();
	//clPrintDevInfo(devices.at(0));
}

bool CLHandler::isValid() {
	return valid;
}

CLHandler::~CLHandler() {
}

