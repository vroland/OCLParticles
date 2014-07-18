/*
 * utils.h
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <string>

using namespace std;

void checkSDLError(int line = -1);
void checkGLError(int line = -1);
void debugMSG(string msg);
void debugERR(string msg);
const char * get_error_string(int err);
std::string convertToString(const char *filename);

#endif /* UTILS_H_ */
