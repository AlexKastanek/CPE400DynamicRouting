/*
 * frameBuffer.cpp
 *
 *  Created on: Nov 21, 2018
 *      Author: Ronn Quijada
 */

#include "frameBuffer.h"

//define the data for a quad
static const GLfloat quad[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
};

FrameBuffer::FrameBuffer(int width, int height) {
	this->width = width;
	this->height = height;
	depthBuffer = 0;

	//bind the data for the quad
	glGenBuffers(1, &quadData);
	glBindBuffer(GL_ARRAY_BUFFER, quadData);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	cout << sizeof(quad) << endl;

	//Generate pointers for frame buffer object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//generate pointers for target texture
	glGenTextures(1, &FBOutput);
	glBindTexture(GL_TEXTURE_2D, FBOutput);

	//setup texture (similar to how we setup textures in Texture Loading)
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//generate pointers for target depth
	glGenTextures(1, &depthOutput);
	glBindTexture(GL_TEXTURE_2D, depthOutput);

	//setup texture (similar to how we setup textures in Texture Loading)
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 


	// Attach our empty texture to the 0th entry of our Frame buffer object
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, FBOutput, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthOutput, 0);

	// Set the list of draw buffers (currently only one)
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
	glDrawBuffers(1, DrawBuffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "FRAME BUFFER WAS SETUP INCORRECTLY" << endl;
	}

}

FrameBuffer::~FrameBuffer() {

}

void FrameBuffer::useBuffer() {
	//bind the frame buffer to render to it
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0,0,width,height);
}

void FrameBuffer::useOutput(GLuint target) {
	//bind the texture to a texture target
	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_2D, FBOutput);
}

void FrameBuffer::useDepthOutput(GLuint target) {
	//bind the texture to a texture target
	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_2D, depthOutput);
}

void FrameBuffer::renderToScreen() {
	//render the frame buffer onto the screen
	//assume the screen shader has been enabled

	glEnableVertexAttribArray(0);

	//bind the quad
	glBindBuffer(GL_ARRAY_BUFFER, quadData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//bind the texture
	useOutput(GL_TEXTURE0);

	//draw the quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
}


