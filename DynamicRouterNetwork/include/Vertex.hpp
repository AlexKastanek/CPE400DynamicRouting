#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <deque>

using namespace std;

enum EdgeType
{
  WIRED_EDGE = 0,
  WIRELESS_EDGE
};

struct EdgeData
{
  EdgeType type;
  vector<double> costs;

  EdgeData(unsigned int in_type, const vector<double>& in_costs)
  {
    type = static_cast<EdgeType>(in_type);
    costs = in_costs;
  };
};

template <class T>
class Vertex
{  

public:

  /**
  * Name: Edge
  * Description: A vertex edge. Made up of an instance of EdgeData 
  *   and a pointer to the other vertex. This will effectively hold 
  *   the info of a connection between this vertex and the other vertex. 
  *   The double stores the cost or weight info of this edge.
  */
  typedef pair<EdgeData, Vertex<T>*> Edge;

  //Constructor
  Vertex(int id);
  Vertex(int id, const T& data);
  //Destructor
  ~Vertex();

  bool IsConnectedTo(int id);

  vector<double>& GetEdgeCosts(int id);
  double GetEdgeCost(int id);

  T& GetData();
  int GetID();

  void SetData(const T& data);

  void SetEdgeCosts(int id, const vector<double>& edgeCosts);

  deque<Edge> m_adjacencyList;

protected:

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
vector<double>& Vertex<T>::GetEdgeCosts(int id)
{
  for (int i = 0; i < m_adjacencyList.size(); i++)
  {
    if (m_adjacencyList[i].second->GetID() == id)
    {
      return m_adjacencyList[i].first.costs;
    }
  }

  throw runtime_error("ERROR: id does not exist in this adjacency list");
  //cout << "ERROR: id does not exist in this adjacency list" << endl;
}

template <class T>
double Vertex<T>::GetEdgeCost(int id)
{
  for (int i = 0; i < m_adjacencyList.size(); i++)
  {
    if (m_adjacencyList[i].second->GetID() == id)
    {
      double totalEdgeCost = 0;
      for (int j = 0; j < m_adjacencyList[i].first.costs.size(); j++)
      {
        totalEdgeCost += m_adjacencyList[i].first.costs[j];
      }
      return totalEdgeCost;
    }
  }

  throw runtime_error("ERROR: id does not exist in this adjacency list");
  //cout << "ERROR: id does not exist in this adjacency list" << endl;
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

template <class T>
void Vertex<T>::SetEdgeCosts(int id, const vector<double>& edgeCosts)
{
  for (int i = 0; i < m_adjacencyList.size(); i++)
  {
    if (m_adjacencyList[i].second->GetID() == id)
    {
      m_adjacencyList[i].first.costs = edgeCosts;
    }
  }
}