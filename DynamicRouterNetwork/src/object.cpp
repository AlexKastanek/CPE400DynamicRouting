#include "object.h"

Object::Object() {
	//updated file loading
	loader fileLoader;
	obj cube;
	fileLoader.loadObject("assets/teapot.ob", cube);
	Vertices = cube.getVerts();
	Indices = cube.getIndices();

	// The index works at a 0th index
	for (unsigned int i = 0; i < Indices.size(); i++) {
		Indices[i] = Indices[i] - 1;
	}

	angle = 0.0f;
	orbit = 0.0f;

	rotationSpeed = 3.0f;
	orbitSpeed = 10.0f;
	distance = 0.0f;

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GraphicsVertex) * Vertices.size(),
			&Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(),
			&Indices[0], GL_STATIC_DRAW);
}

Object::~Object() {
	Vertices.clear();
	Indices.clear();
}

void Object::Update(unsigned int dt) {
	//calculate orbit and convert to position matrix
	orbit -= dt * M_PI / (500 * orbitSpeed);
	xPos = glm::sin(orbit);
	yPos = glm::cos(orbit);
	model = glm::translate(glm::mat4(1.0f),
			glm::vec3(xPos * distance, 0.0, yPos * distance));

	//original rotate code modified to take initial translated matrix
	angle += dt * M_PI / (500 * rotationSpeed);
	model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Object::GetModel() {
	return model;
}

void Object::Render() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicsVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicsVertex),
			(void*) offsetof(GraphicsVertex, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

