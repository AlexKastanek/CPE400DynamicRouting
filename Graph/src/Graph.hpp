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

  //typedef map<int, Vertex<T>*> graph_map;

public:
  //Constructor
  Graph();
  //Destructor
  ~Graph();

  void AddVertex(int id);
  void AddVertex(int id, const T& data);
  void RemoveVertex(int id);
  void ConnectVertices(int from, int to, double cost);
  void DisconnectVertices(int from, int to);

  void Clear();

  void Print();

private:
  map<int, Vertex<T>*> m_map;

};

#endif /* GRAPH_H */

template <class T>
Graph<T>::Graph()
{

}

template <class T>
Graph<T>::~Graph()
{

}

template <class T>
void Graph<T>::AddVertex(int id)
{
  typename map<int, Vertex<T>*>::iterator it = m_map.find(id);

  if (it == m_map.end())
  {
    Vertex<T>* vertex = new Vertex<T>(id);
    m_map[id] = vertex;
  }
  else
  {
    cout << "Could not add vertex to graph. Vertex already exists" << endl;
  }
}

template <class T>
void Graph<T>::AddVertex(int id, const T& data)
{
  typename map<int, Vertex<T>*>::iterator it = m_map.find(id);
  
  if (it == m_map.end())
  {
    Vertex<T>* vertex = new Vertex<T>(id, data);
    m_map[id] = vertex;
  }
  else
  {
    cout << "Could not add vertex to graph. Vertex already exists" << endl;
  }
}

template <class T>
void Graph<T>::RemoveVertex(int id)
{
  typename map<int, Vertex<T>*>::iterator it;

  //disconnect all vertices connected to this vertex
  for (it = m_map.begin(); it != m_map.end(); it++)
  {
    if (it->second->IsConnectedTo(id))
    {
      DisconnectVertices(it->second->GetID(), id);
    }
  }

  //disconnect all vertices this vertex is connected to
  it = m_map.find(id);
  Vertex<T> *vertex = it->second;
  vertex->m_adjacencyList.clear();

  //remove entry from the map
  m_map.erase(it);
}

template <class T>
void Graph<T>::ConnectVertices(int from, int to, double cost)
{
  Vertex<T> *u = m_map.find(from)->second;
  Vertex<T> *v = m_map.find(to)->second;
  pair<double, Vertex<T>*> edge = make_pair(cost, v);

  u->m_adjacencyList.push_back(edge);
}

template <class T>
void Graph<T>::DisconnectVertices(int from, int to)
{
  Vertex<T> *u = m_map.find(from)->second;
  Vertex<T> *v = m_map.find(to)->second;

  typename deque<typename Vertex<T>::vertex_edge>::iterator it;

  for (it = u->m_adjacencyList.begin(); it != u->m_adjacencyList.end(); it++)
  {
    if (it->second->GetID() == v->GetID())
    {
      u->m_adjacencyList.erase(it);
      return;
    }
  }
}

template <class T>
void Graph<T>::Clear()
{
  typename map<int, Vertex<T>*>::iterator it;

  //remove all vertices
  for (it = m_map.begin(); it != m_map.end(); it++)
  {
    RemoveVertex(it->first);
  }
}

template <class T>
void Graph<T>::Print()
{
  typename map<int, Vertex<T>*>::iterator it;

  for (it = m_map.begin(); it != m_map.end(); it++)
  {
    cout << "Vertex " << it->first << " is connected to vertices:" << endl;
    Vertex<T>* vertex = it->second;
    for (int i = 0; i < vertex->m_adjacencyList.size(); i++)
    {
      cout << vertex->m_adjacencyList[i].second->GetID()
           << " with edge cost " 
           << vertex->m_adjacencyList[i].first
           << endl;
    }
    cout << endl;
  }
}