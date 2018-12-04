/*
 * loader.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#include <loader.h>

loader::loader() {
	isEOF = false;
	isAnnoying = false;
}

loader::~loader() {
	isEOF = false;
	isAnnoying = false;
}

bool loader::loadObject(string filename, obj &inputObj) {
	obj output;
	char type;
	filein.open(filename.c_str());

	if (filein.fail()) {
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
		return false;
	}

	isEOF = false;
	//walk through file line by line
	//read starting character
	filein >> type;
	while (!isEOF) {
		//read line and return next
		if (isAnnoying)
			cout << type << "| ";
		readLine(filein, type, output);
	}
	//build object normals and create vertices
	calculateNormals(output);

	filein.close();
	inputObj = output;
	return true;
}

bool loader::loadShader(string filename, string& output) {
	isEOF = false;
	string final = "";
	filein.open(filename.c_str());

	if (filein.fail()) {
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
		return false;
	}

	//load file
	while (!isEOF) {
		if (filein.eof()) {
			isEOF = true;
			break;
		}
		final += filein.get();
	}

	//return file
	final = final.substr(0, final.length() - 1);
	cout << output.size() << endl;
	output = final;

	filein.close();
	return true;
}

void loader::readLine(fstream &file, char &current, obj &object) {
	switch (current) {
	case '#':
		readComment(file, current, object);
		break;
	case 'o':
		readObj(file, current, object);
		break;
	case 'v':
		readVertex(file, current, object);
		break;
	case 's':
		readSection(file, current, object);
		break;
	case 'f':
		readIndice(file, current, object);
		break;
	default:
		current = file.get();
		if (isAnnoying)
			cout << endl;
		if (current == -1)
			isEOF = true;
		break;
	}
}

void loader::readComment(fstream &file, char &current, obj &object) {
	char read;
	//ignore everything
	while (true) {
		read = file.get();
		if (isAnnoying)
			cout << read;

		if (read == '\n' || read == '\r') {
			break;
		}

		if (read == -1) {
			isEOF = true;
			break;
		}
	}
	if (isAnnoying)
		cout << endl;
	current = file.get();
}

void loader::readObj(fstream &file, char &current, obj &object) {
	string read;
	//assume it has no spaces
	//currently does nothing
	file >> read;
	if (isAnnoying)
		cout << read << endl;

	//next char
	file >> read;
	current = read.c_str()[0];
}

void loader::readVertex(fstream &file, char &current, obj &object) {
	glm::vec3 vect;
	float reader;
	//read in three float values
	file >> vect.x;
	file >> vect.y;
	file >> vect.z;

	//build vertex and add
	object.addRaw(vect);
	if (isAnnoying)
		cout << vect.x << "," << vect.y << "," << vect.z << endl;

	//load next
	file >> current;
}

void loader::readSection(fstream &file, char &current, obj &object) {
	string read;
	//assume it has no spaces
	//currently does nothing
	file >> read;
	if (isAnnoying)
		cout << read << endl;
	//next char
	file >> current;
}

void loader::readIndice(fstream &file, char &current, obj &object) {
	unsigned int readin;
	for (int i = 0; i < 3; i++) {
		file >> readin;
		object.addIndice(readin);
		if (isAnnoying)
			cout << readin << ",";
	}

	//next char
	current = file.get();
	if (current == -1)
		isEOF = true;
}

void loader::calculateNormals(obj &object) {
	glm::vec3 vert1;
	glm::vec3 vert2;
	glm::vec3 vert3;
	glm::vec3 final;

	GraphicsVertex *temp = NULL;

	vector<unsigned int> indices = object.getIndices();
	vector<glm::vec3> vertices = object.getRawVerts();
	//iterate through all faces and build vertex normals
	for (int i = 0; i < indices.size(); i += 3) {
		//load all three vertices
		//get indices-1 to get the correct vertex index

		if (isAnnoying)
			cout << "loaded" << indices[i] << indices[i + 1] << indices[i + 2]
					<< endl;
		vert1 = vertices[indices[i] - 1];
		vert2 = vertices[indices[i + 1] - 1];
		vert3 = vertices[indices[i + 2] - 1];

		//calculate face normal
		final = glm::cross(vert2 - vert1, vert3 - vert1);
		final = glm::normalize(final);
		if (isAnnoying)
			cout << final.x << " " << final.y << " " << final.z << endl;

		//add to all affected vertices
		//normalize with any existing normals
		if (isAnnoying)
			cout << "built vert at" << indices[i] - 1 << endl;

		temp = new GraphicsVertex(vert1, final);
		object.addVert(*temp, indices[i] - 1);
		delete temp;

		temp = new GraphicsVertex(vert2, final);
		object.addVert(*temp, indices[i + 1] - 1);
		delete temp;

		temp = new GraphicsVertex(vert3, final);
		object.addVert(*temp, indices[i + 2] - 1);
		delete temp;
		temp = NULL;
	}
}

bool loader::isHeader(string test) {
	char extracted = test.c_str()[0];

	switch (extracted) {
	case '#':
	case 'o':
	case 'v':
	case 's':
	case 'f':
		return true;
	default:
		return false;
	}
}
