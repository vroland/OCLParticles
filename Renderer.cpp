/*
 * Renderer.cpp
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#include "Renderer.h"
#include "utils.h"

#include <iostream>

using namespace std;

Renderer::Renderer() {
	lastTime = SDL_GetTicks();
	tick_time = 0;
	ticks = 0;

	rotationX = 0;
	rotationY = 0;
	zoom	  = -3;
	cam_pos	  = vec3(0,0,0);
}


void Renderer::glInit(int width, int height) {

	//Viewport aktualisieren
	glClearColor(0.0, 0.0, 0.0, 0.0);
	resize(width,height);
	glEnable( GL_DEPTH_TEST );
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

}

void Renderer::resize(int width, int height) {
	//Viewport aktualisieren
	glViewport(0, 0, width, height);

	//Projektion aktualisieren
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double h = (double) height / (double) width;
	glFrustum(-1.0, 1.0, -h, h, 1.5, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/**
 * Zeichnet markante Linien zum leichteren Erfassen des Koordinatensystems.
 */
void renderGrid(size_t grid_size) {
	glColor4f(.2, .2, .2, .2);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	//Linien in z-Richtung zeichnen
	for (size_t i = 0; i < grid_size; i++) {
		glVertex3i(i, 0, grid_size);
		glVertex3i(i, 0, -grid_size);
		glVertex3i(-i, 0, grid_size);
		glVertex3i(-i, 0, -grid_size);
	}
	//Linien in x-Richtung zeichnen
	for (size_t i = 0; i < grid_size; i++) {
		glVertex3i(grid_size, 0, i);
		glVertex3i(-grid_size, 0, i);
		glVertex3i(grid_size, 0, -i);
		glVertex3i(-grid_size, 0, -i);
	}
	glEnd();
}

void Renderer::renderGravObj(ParticleSystem::GravObj obj) {
	glColor4f(1,1,1,1);
	glPushMatrix();
	glTranslatef(obj.x,obj.y,obj.z);
	glutSolidSphere(.1, 20, 20);
	glPopMatrix();
}

void Renderer::render(ParticleSystem* sys) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0.0,0.0, zoom*abs(zoom));
	glRotatef(rotationY,1.0,0.0,0.0);
	glRotatef(rotationX,0.0,1.0,0.0);
	glTranslatef(cam_pos.x,cam_pos.y,cam_pos.z);
	//cout << zoom << " "<< rotationX << " " << rotationY << endl;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat DiffuseLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat AmbientLight[] = { .1, .1, .1 };
	GLfloat LightPosition[] = { -100, 100, 100, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);

	sys->calc();

	//renderGrid(100);
	for (size_t i=0;i<sys->grav_obs.size();i++) {
		renderGravObj(sys->grav_obs.at(i));
	}

	ticks++;
	if (!(ticks%20)) {
		tick_time = (SDL_GetTicks()-lastTime)/20000.f;
		lastTime = SDL_GetTicks();
		SDL_SetWindowTitle(SDL_GL_GetCurrentWindow(),to_string(1/tick_time).c_str());
	}
	glDisable(GL_LIGHTING);

	glBindBuffer(GL_ARRAY_BUFFER, sys->vbo);

	glUseProgram(sys->particleProgram);


	checkGLError(__LINE__);

	glInterleavedArrays(GL_V3F, sizeof(ParticleSystem::Particle), NULL);
	checkGLError(__LINE__);

	glDrawArrays(GL_POINTS, 0, sys->particleCount);

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnable(GL_LIGHTING);

	checkGLError(__LINE__);
}


Renderer::~Renderer() {

}

