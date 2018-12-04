/*
 * frameBuffer.h
 *
 *  Created on: Nov 21, 2018
 *      Author: Ronn Quijada
 */

#ifndef INCLUDE_FRAMEBUFFER_H_
#define INCLUDE_FRAMEBUFFER_H_

#include <iostream>
using namespace std;

#include "graphics_headers.h"

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();
	void useBuffer();
	void useOutput(GLuint target);
	void useDepthOutput(GLuint target);
	void renderToScreen();
private:
	GLuint FBO;
	GLuint FBOutput;
	GLuint depthOutput;
	GLuint depthBuffer;
	GLuint quadData;
	int width;
	int height;
};



#endif /* INCLUDE_FRAMEBUFFER_H_ */
