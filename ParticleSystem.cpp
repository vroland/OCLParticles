/*
 * ParticleSystem.cpp
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#include "ParticleSystem.h"
#include <iostream>
#include <cmath>
#include "utils.h"

using namespace std;

cl_float grav_obj_init[][4] = {
	{0,0,0, 10},
	{3,3,1, 20},
	{-3, 2, .4, 15}
};

float sqr(float x) {
	return x*x;
}
int checkCompileStatus(GLuint shaderObject) {
	GLint compiled;

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint blen = 0;
		GLsizei slen = 0;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &blen);
		GLchar* compiler_log = (GLchar*) malloc(blen);
		glGetShaderInfoLog(shaderObject, blen, &slen, compiler_log);
		cout << "compiler_log:\n" << compiler_log << endl;
		free(compiler_log);
		return -1;
	} else {
		return 0;
	}
}

ParticleSystem::ParticleSystem(CLHandler* clHandle, size_t sphere_layers, float radius, float point_dist) {

	glFinish();

	valid = true;

	cl_int status = 0;
	for (size_t o = 0; o < sizeof(grav_obj_init) / (4 * sizeof(cl_float)); o++) {
		GravObj obj;
		obj.x = grav_obj_init[o][0];
		obj.y = grav_obj_init[o][1];
		obj.z = grav_obj_init[o][2];
		obj.mass = grav_obj_init[o][3];
		grav_obs.resize(grav_obs.size()+1,obj);
	}


	size_t p_count = 0;
	for (size_t layer = 0; layer < sphere_layers; layer++) {
		float layer_radius = sqrt(1. - sqr(2 * (.5 - (float) layer / sphere_layers))) * radius;
		p_count += layer_radius * 2 * M_PI / point_dist;
	}

	particleCount = p_count;
	cout << p_count << endl;

	ticks = 0;
	tick_time = 0;
	birthTime = SDL_GetTicks();

	device = clHandle->devices.at(0);


	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBufferData(GL_ARRAY_BUFFER, particleCount*sizeof(Particle), NULL, GL_STATIC_DRAW);

	checkGLError(__LINE__);

	Particle* vbo_pointer = (Particle*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	//input = new cl_float[particleCount * PARTICLE_ATTRIBUTE_COUNT];
	for (size_t layer=0;layer<sphere_layers;layer++) {
		float layer_radius = sqrt(1. - sqr(2 * (.5 - (float) layer / sphere_layers))) * radius;
		int point_count = layer_radius * 2 * M_PI / point_dist;

		for (int point = 0; point < point_count; point++) {
			//position
			vbo_pointer->x = -10+sin((float)point/point_count*2*M_PI)*layer_radius +rand() % 100 / 10000. * radius;
			vbo_pointer->y = -10+(float)layer/sphere_layers * 2 *radius          +rand() % 100 / 10000. * radius;
			vbo_pointer->z = cos((float)point/point_count*2*M_PI)*layer_radius +rand() % 100 / 10000. * radius;
			vbo_pointer->w = 0;

			//velocity
			vbo_pointer->v1 = 0; //(rand()%1000/1000.-.5)*.1;
			vbo_pointer->v2 = 0; //(rand()%1000/1000.-.5)*.1;
			vbo_pointer->v3 = 0; //(rand()%1000/1000.-.5)*.1;
			vbo_pointer->v4 = 0;

			//attributes
			vbo_pointer->m = .01;	//mass rand() % 1000 / 10000. +
			vbo_pointer->r = 1.;	//r
			vbo_pointer->g = .1;	//g
			vbo_pointer->b = 0;	//b

			//cout << ((float*) vbo_pointer)[0]<< " "<<((float*) vbo_pointer)[1]<<" "<<((float*) vbo_pointer)[2] << endl;
			vbo_pointer++;
		}
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	string VertexShaderSource = convertToString("vertex_shader.glsl");
	string FragmentShaderSource = convertToString("fragment_shader.glsl");
	const char *vertShaderSrc = VertexShaderSource.c_str();
	const char *fragShaderSrc = FragmentShaderSource.c_str();

	glShaderSource(vertexShaderObject, 1, &vertShaderSrc, NULL);
	glShaderSource(fragmentShaderObject, 1, &fragShaderSrc, NULL);

	glCompileShader(vertexShaderObject);
	glCompileShader(fragmentShaderObject);

	checkCompileStatus(vertexShaderObject);
	checkCompileStatus(fragmentShaderObject);

	particleProgram = glCreateProgram();

	glAttachShader(particleProgram, vertexShaderObject);
	glAttachShader(particleProgram, fragmentShaderObject);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	checkGLError(__LINE__);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), (void*)(sizeof(GL_FLOAT)*4));	//velocity
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), (void*)(sizeof(GL_FLOAT)*8));	//mass
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), (void*)(sizeof(GL_FLOAT)*9));	//color

	checkGLError(__LINE__);

	glBindAttribLocation(particleProgram, 1, "particleVelocity");
	glBindAttribLocation(particleProgram, 2, "particleMass");
	glBindAttribLocation(particleProgram, 3, "particleColor");

	checkGLError(__LINE__);

	glLinkProgram(particleProgram);

	GLint linked;
	glGetProgramiv(particleProgram, GL_LINK_STATUS, &linked);
	if (!linked) {
		checkGLError(__LINE__);
		cout << "shader link error!" << endl;
	}
	checkGLError(__LINE__);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	data_buffer = cl::BufferGL(clHandle->context, CL_MEM_READ_WRITE, vbo, &status);

	if(status != CL_SUCCESS)
	{
		cout << get_error_string(status) << endl;
		std::cout << "Error: clCreateBuffer (data_buffer)\n";
		valid = false;
		return;
	}

	grav_obj_buffer = cl::Buffer(grav_obs.begin(),grav_obs.end(),true,CL_MEM_USE_HOST_PTR, &status);
	if (status != CL_SUCCESS) {
		cout << get_error_string(status) << endl;
		std::cout << "Error: clCreateBuffer (grav_obj_buffer)\n";
		valid = false;
		return;
	}

	/////////////////////////////////////////////////////////////////
	// STEP 6. Building Kernel
	//      6.1 Load CL file, using basic file i/o
	//      6.2 Build CL program object
	//      6.3 Create CL kernel object
	/////////////////////////////////////////////////////////////////
	const char * filename  = "ParticleKernel.cl";
	std::string  sourceStr = convertToString(filename);
	program = cl::Program(clHandle->context, sourceStr);
	status = program.build(clHandle->devices,NULL,NULL,NULL);

	if (status != CL_SUCCESS) {
		std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(cl::Device::getDefault()) << std::endl;
		std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(cl::Device::getDefault()) << std::endl;
		std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(cl::Device::getDefault()) << std::endl;
		valid = false;
		return;
	}

	kernel = cl::Kernel(program, "templateKernel", &status);
	// get a kernel object handle for a kernel with the given name
	if(status != CL_SUCCESS)
	{
		std::cout << get_error_string(status) <<"Error: Creating Kernel from program. (clCreateKernel)\n";
		valid = false;
		return;
	}

	// the output array to the kernel

	status = kernel.setArg(0,data_buffer);
	status = kernel.setArg(1,tick_time);
	status = kernel.setArg(2,0);
	status = kernel.setArg(3,particleCount);
	status = kernel.setArg(4,grav_obj_buffer);
	status = kernel.setArg(5,grav_obs.size());

	commandQueue = cl::CommandQueue(clHandle->context, device, 0, &status);

	if (status != CL_SUCCESS) {
		std::cout << "command queue\n";
		valid = false;
		return;
	}

	lastTime = SDL_GetTicks();
}

bool ParticleSystem::isValid() {
	return valid;
}
void ParticleSystem::calc() {

	cl_int status;

	ticks++;

	glFinish();

	cl::Event event;

	vector<cl::Memory> buffers = vector<cl::Memory>(1);
	buffers.at(0) = (cl::Memory) data_buffer;

	status = commandQueue.enqueueAcquireGLObjects(&buffers,NULL,NULL);

	if (status != CL_SUCCESS) {
		std::cout << get_error_string(status) << endl;
		std::cout
				<< "Error: Enqueueing acquisition of GL objects. \
			   (clEnqueueReleaseGLObjects)\n";
		return;
	}

	status = commandQueue.finish();

	if (status != CL_SUCCESS) {
		std::cout << get_error_string(status) << endl;
		std::cout
				<< "Error: Enqueueing acquisition of GL objects. \
				   (clEnqueueReleaseGLObjects)\n";
		return;
	}


	if (!(ticks%20)) {
		tick_time = (SDL_GetTicks()-lastTime)/20000.f;
		lastTime = SDL_GetTicks();
		status = kernel.setArg(1,tick_time);
		if(status != CL_SUCCESS)
		{
			std::cout << "Error: Setting kernel argument. (multiplier)\n";
			return;
		}
	}
	size_t age = SDL_GetTicks()-birthTime;
	status = kernel.setArg(2,age);

    ////////////////////////////////////////////////////////////////////
    // STEP 9 Enqueue a kernel run call.
    //          Wait till the event completes and release the event
    ////////////////////////////////////////////////////////////////////
	status = commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(particleCount), cl::NullRange, NULL, &event);

    if(status != CL_SUCCESS)
    {
    	std::cout << get_error_string(status) << endl;
        std::cout <<
            "Error: Enqueueing kernel onto command queue. \
            (clEnqueueNDRangeKernel)\n";
        return;
    }

    // wait for the kernel call to finish execution
	status = event.wait();
	if (status != CL_SUCCESS) {
		std::cout << get_error_string(status) << endl;
		std::cout << "Error: finishing kernel\n";
		return;
	}

	status = commandQueue.enqueueReleaseGLObjects(&buffers, NULL, NULL);
	if (status != CL_SUCCESS) {
		std::cout << get_error_string(status) << endl;
		std::cout
				<< "Error: Enqueueing release of GL objects. \
		   (clEnqueueReleaseGLObjects)\n";
		return;
	}

	commandQueue.finish();

}

ParticleSystem::~ParticleSystem() {
	glDetachShader(particleProgram,vertexShaderObject);
	glDetachShader(particleProgram,fragmentShaderObject);
	glDeleteProgram(particleProgram);
	glDeleteShader (vertexShaderObject);
	glDeleteShader (fragmentShaderObject);
	glFinish();

	glDeleteBuffers(1, &vbo);
}

