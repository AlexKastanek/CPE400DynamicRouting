#ifndef ROUTER_H
#define ROUTER_H

#include "Vertex.hpp"

using namespace std;

template <class T>
class Router : public Vertex
{

public:

  //Constructor
  Router(int id);
  Router(int id, const T& data);
  //Destructor
  ~Router();

  double GetNodalProcessingDelay();
  double GetQueuingDelay();

  void SetNodalProcessingDelay(double nodalProcessingDelay);
  void SetQueuingDelay(double queuingDelay);

private:

  double m_nodalProcessingDelay;
  double m_queuingDelay;

}

#endif /* ROUTER_H */

template <class T>
Router<T>::Router(int id) : Vertex(id)
{

}

template<class T>
Router<T>::Router(int id, const T& data) : Vertex(id, data)
{

}

template <class T>
Router<T>::~Router()
{
  delete m_data;
  m_adjacencyList.clear();
}

template <class T>
double Router<T>::GetNodalProcessingDelay()
{
  return m_nodalProcessingDelay;
}

template <class T>
double Router<T>::GetQueuingDelay()
{
  return m_queuingDelay;
}

template <class T>
void Router<T>::SetNodalProcessingDelay(double nodalProcessingDelay)
{
  m_nodalProcessingDelay = nodalProcessingDelay;
}

template <class T>
void Router<T>::SetQueuingDelay(double queuingDelay)
{
  m_queuingDelay = queuingDelay;
}
