#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include <deque>

using namespace std;

template <class T>
class Vertex
{  
public:
  /**
  * Name: vertex_edge
  * Description: A vertex edge. Made up of a double and a pointer to the
  *   the other vertex. This will effectively hold the info of a connection
  *   between this vertex and the other vertex. The double stores the cost
  *   or weight info of this edge.
  */
  typedef pair<double, Vertex<T>*> vertex_edge;

  //Constructor
  Vertex(int id);
  Vertex(int id, const T& data);
  //Destructor
  ~Vertex();

  bool IsConnectedTo(int id);

  T& GetData();
  int GetID();

  void SetData(const T& data);

  deque<vertex_edge> m_adjacencyList;

private:
  T* m_data;
  int m_id;
};

#endif /* VERTEX_H */

template <class T>
Vertex<T>::Vertex(int id)
{
  m_data = new T();
  m_id = id;
  m_adjacencyList.clear();
}

template <class T>
Vertex<T>::Vertex(int id, const T& data)
{
  m_data = new T(data);
  m_id = id;
  m_adjacencyList.clear();
}

template <class T>
Vertex<T>::~Vertex()
{
  delete m_data;
  m_adjacencyList.clear();
}

template <class T>
bool Vertex<T>::IsConnectedTo(int id)
{
  for (int i = 0; i < m_adjacencyList.size(); i++)
  {
    if (m_adjacencyList[i].second->GetID() == id)
    {
      return true;
    }
  }
  return false;
}

template <class T>
T& Vertex<T>::GetData()
{
  return *m_data;
}

template <class T>
int Vertex<T>::GetID()
{
  return m_id;
}

template <class T>
void Vertex<T>::SetData(const T& data)
{
  *m_data = data;
}