/*
 * Renderer.h
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glut.h>

#include "ParticleSystem.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Renderer {
public:
	Renderer();

	void glInit(int width, int height);
	void resize(int width, int height);
	void renderGravObj(ParticleSystem::GravObj obj);
	void render(ParticleSystem* sys);

	vec3 cam_pos;
	float rotationX;
	float rotationY;
	float zoom;

	virtual ~Renderer();
private:

	size_t lastTime;
	size_t ticks;
	float tick_time; //average tick time in ms
};

#endif /* RENDERER_H_ */
