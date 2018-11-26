#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include <deque>

using namespace std;

template <class T>
class Vertex
{
  typedef pair<double, Vertex<T>*> vertex_edge;

public:
  //Constructor
  Vertex();
  Vertex(const T& data);
  //Destructor
  ~Vertex();

  T& GetData() const;

  void SetData(const T& data);

  deque<vertex_edge> m_adjacencyList;

private:
  T* m_data;
};

#endif /* VERTEX_H */

template <class T>
Vertex<T>::Vertex()
{
  m_data = new T();
  m_adjacencyList.clear();
}

template <class T>
Vertex<T>::Vertex(const T& data)
{
  m_data = new T(data);
  m_adjacencyList.clear();
}

template <class T>
Vertex<T>::~Vertex()
{
  delete m_data;
  m_adjacencyList.clear();
}

template <class T>
T& Vertex<T>::GetData() const
{
  return *m_data;
}

template <class T>
void Vertex<T>::SetData(const T& data)
{
  *m_data = data;
}