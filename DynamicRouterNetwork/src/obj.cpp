/*
 * obj.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#include <obj.h>

obj::obj() {
}

obj::~obj() {
	vertRaw.empty();
	vertices.empty();
	indices.empty();
}

void obj::addVert(GraphicsVertex vert) {
	vertices.push_back(vert);
}

void obj::addVert(GraphicsVertex vert, int index) {
	vector<GraphicsVertex>::iterator i;

	//insert if it doesn't yet exist
	i = vertices.begin() + index;
	if (vertices.at(index).vertex != vert.vertex) {
		vertices.erase(i);
		vertices.insert(i, vert);
	} else {
		//otherwise, average normals
		GraphicsVertex temp = vertices.at(index);
		temp.color = glm::normalize(temp.color + vert.color);
		vertices.erase(i);
		vertices.insert(i, temp);
	}
}

void obj::addIndice(unsigned int ind) {
	indices.push_back(ind);
}

void obj::addRaw(glm::vec3 raw) {
	vertRaw.push_back(raw);

	//allocate size for new vertex in vertices
	glm::vec3 item;
	GraphicsVertex temp(item, item);
	vertices.push_back(temp);
}

vector<GraphicsVertex> obj::getVerts() {
	return vertices;
}

vector<unsigned int> obj::getIndices() {
	return indices;
}

vector<glm::vec3> obj::getRawVerts() {
	return vertRaw;
}