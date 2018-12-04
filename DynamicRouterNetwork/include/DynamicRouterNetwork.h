#ifndef DYNAMIC_ROUTER_NETWORK_H
#define DYNAMIC_ROUTER_NETWORK_H

#include "Graph.hpp"
#include "Router.h"
#include <time.h>
#include <math.h>
#include <stack>
#include <iostream>

using namespace std;

class DynamicRouterNetwork
{

public:

  //Constructor
  DynamicRouterNetwork();
  //Destructor
  ~DynamicRouterNetwork();

  void Initialize();
  void Update();

  void AddRouter();
  void RemoveRouter(int id);
  void ChangeRouters();
  void GenerateEdgeCosts(vector<double>& edgeCosts);

  bool VertexExists(int id);
  bool PathExists(int from, int to);

  void Print();

  vector<int>* BestPathDijsktra(int from, int to);
  vector<int>* BestPathBellmanFord(int from, int to);

  Graph<Router>& GetGraph();

private:

  Graph<Router> m_graph;
  int m_currentRouterIndex = -1;

};

#endif /* DYNAMIC_ROUTER_NETWORK_H */