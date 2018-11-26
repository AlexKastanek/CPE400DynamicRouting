#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <map>
#include "Vertex.hpp"

using namespace std;

template <class T>
class Graph
{
public:
  //Constructor
  Graph();
  //Destructor
  ~Graph();

  void AddVertex(int id);
  void AddVertex(int id, const T& data);
  void RemoveVertex(int id);
  void ConnectVertices(int from, int to, double cost);
  void DisconnectVertices(int from, int to, double cost);

  void Clear();

  void Print();

private:
  map<int, Vertex<T>*> m_map;

};

#endif /* GRAPH_H */

template <class T>
Graph::Graph()
{

}

template <class T>
Graph::~Graph()
{

}

template <class T>
void Graph::AddVertex(int id)
{
  map::iterator it = m_map.find(id);

  if (it == m_map.end())
  {
    Vertex<T>* vertex = new Vertex(id);
    m_map[id] = vertex;
  }
  else
  {
    cout << "Coult not add vertex to graph. Vertex already exists" << endl;
  }
}

template <class T>
void Graph::AddVertex(int id, const T& data)
{
  map::iterator it = m_map.find(id);
  
  if (it == m_map.end())
  {
    Vertex<T>* vertex = new Vertex(id, data);
    m_map[id] = vertex;
  }
  else
  {
    cout << "Coult not add vertex to graph. Vertex already exists" << endl;
  }
}

template <class T>
void Graph::RemoveVertex(int id)
{

}

template <class T>
void Graph::ConnectVertices(int from, int to, double cost)
{

}

template <class T>
void Graph::DisconnectVertices(int from, int to, double cost)
{

}

template <class T>
void Graph::Clear()
{

}

template <class T>
void Graph::Print()
{

}