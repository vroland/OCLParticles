/*
 * ParticleSystem.h
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#define GL_GLEXT_PROTOTYPES
#include "CLHandler.h"
#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <CL/cl.hpp>


#define PARTICLE_ATTRIBUTE_COUNT 12

class ParticleSystem {
public:
	ParticleSystem(CLHandler* clHandle, size_t sphere_layers, float radius, float point_dist);

	void calc();

	GLuint particleProgram; //particle shader program
	size_t particleCount;
	GLuint vbo;

	bool isValid();

	struct Particle {
		cl_float x;
		cl_float y;
		cl_float z;
		cl_float w;		//first  float4
		cl_float v1;
		cl_float v2;
		cl_float v3;
		cl_float v4;	//second float4
		cl_float m;
		cl_float r;
		cl_float g;
		cl_float b;		//third  float4
	}__attribute__ ((packed));

	struct GravObj {
		cl_float x;
		cl_float y;
		cl_float z;
		cl_float mass;
	}__attribute__ ((packed));

	std::vector<GravObj> grav_obs;

	virtual ~ParticleSystem();

private:
	bool valid;

	GLuint vertexShaderObject, fragmentShaderObject;
	cl::CommandQueue commandQueue;
	cl::BufferGL data_buffer;
	cl::Device device;
	cl::Program program;
	cl::Kernel  kernel;
	cl::Buffer grav_obj_buffer;

	size_t lastTime;
	size_t birthTime;		//time of birth in ms after SDL start
	size_t ticks;
	float tick_time; //average tick time in ms
};

#endif /* PARTICLESYSTEM_H_ */
