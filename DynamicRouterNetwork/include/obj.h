/*
 * obj.h
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#ifndef OBJ_H_
#define OBJ_H_

#include <vector>
#include "graphics_headers.h"
using namespace std;

class obj {
private:
	vector<glm::vec3> vertRaw;
    vector<GraphicsVertex> vertices;
    vector<unsigned int> indices;
public:
    obj();
    ~obj();

    void addVert(GraphicsVertex);
    void addVert(GraphicsVertex, int);
    void addIndice(unsigned int);
    void addRaw(glm::vec3);

    vector<GraphicsVertex> getVerts();
    vector<unsigned int> getIndices();
    vector<glm::vec3> getRawVerts();
};




#endif /* OBJ_H_ */
