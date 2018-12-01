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
  Vertex<T>* GetVertexWithID (int id);
  void RemoveVertex(int id);
  void ConnectVertices(int from, int to, 
    unsigned int edgeType, 
    const vector<double>& edgeCosts);
  void DisconnectVertices(int from, int to);

  void Clear();

  void Print();

  map<int, Vertex<T>*>& GetMap();

  void SetVertexData(int id, const T& data);
  void SetVertexEdgeCosts(int from, int to, const vector<double>& edgeCosts);

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

  //allocate memory for a new vertex and add it to the map if it does not
  //already exist
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
  
  //allocate memory for a new vertex and add it to the map if it does not
  //already exist
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
Vertex<T>* Graph<T>::GetVertexWithID (int id)
{
  typename map<int, Vertex<T>*>::iterator it;

  it = m_map.find (id);

  if (it != m_map.end ())
  {
    return it->second;

  } else
  {
    return NULL;

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
void Graph<T>::ConnectVertices(int from, int to, unsigned int edgeType, const vector<double>& edgeCosts)
{
  Vertex<T> *u = m_map.find(from)->second;
  Vertex<T> *v = m_map.find(to)->second;

  EdgeData edgeData(edgeType, edgeCosts);

  pair<EdgeData, Vertex<T>*> edge = make_pair(edgeData, v);

  //add the new edge to the from vertex's adjacency list
  u->m_adjacencyList.push_back(edge);
}

template <class T>
void Graph<T>::DisconnectVertices(int from, int to)
{
  Vertex<T> *u = m_map.find(from)->second;
  Vertex<T> *v = m_map.find(to)->second;

  typename deque<typename Vertex<T>::Edge>::iterator it;

  //erase the matching edge from the from vertex's adjacency list
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
      //add up each componenet of the vertex's edge cost
      double totalEdgeCost = 0;
      vector<double> edgeCosts = vertex->m_adjacencyList[i].first.costs;
      for (int j = 0; j < edgeCosts.size(); j++)
      {
        totalEdgeCost += edgeCosts[j];
      }

      cout << vertex->m_adjacencyList[i].second->GetID()
           << " with edge cost " 
           << totalEdgeCost
           << endl;
    }
    cout << endl;
  }
}

template <class T>
map<int, Vertex<T>*>& Graph<T>::GetMap()
{
  return m_map;
}

template <class T>
void Graph<T>::SetVertexData(int id, const T& data)
{
  Vertex<T>* vertex = m_map.find(id)->second;
  vertex->SetData(data);
}

template <class T>
void Graph<T>::SetVertexEdgeCosts(int from, int to, const vector<double>& edgeCosts)
{
  Vertex<T>* vertex = m_map.find(from)->second;
  vertex->SetEdgeCosts(to, edgeCosts);
}