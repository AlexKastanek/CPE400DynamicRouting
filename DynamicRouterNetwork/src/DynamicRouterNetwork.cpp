#include "DynamicRouterNetwork.h"

DynamicRouterNetwork::DynamicRouterNetwork()
{

}

DynamicRouterNetwork::~DynamicRouterNetwork()
{

}

void DynamicRouterNetwork::Initialize()
{
  //initialize single vectors with one value
  /*
  vector<double> one(1, 1);
  vector<double> two(1, 2);
  vector<double> three(1, 3);
  vector<double> four(1, 4);

  graph->AddVertex(0, 'A');
  graph->AddVertex(1, 'B');
  graph->AddVertex(2, 'C');
  graph->AddVertex(3, 'D');
  graph->AddVertex(4, 'E');
  graph->AddVertex(5, 'F');

  //from and to vertex A
  graph->ConnectVertices(0, 1, 0, two);
  graph->ConnectVertices(1, 0, 0, two);

  graph->ConnectVertices(0, 3, 0, one);
  graph->ConnectVertices(3, 0, 0, one);

  //from and to vertex B
  graph->ConnectVertices(1, 2, 0, one);
  graph->ConnectVertices(2, 1, 0, one);

  graph->ConnectVertices(1, 3, 0, two);
  graph->ConnectVertices(3, 1, 0, two);

  graph->ConnectVertices(1, 4, 0, three);
  graph->ConnectVertices(4, 1, 0, three);

  //from and to vertex C
  graph->ConnectVertices(2, 4, 0, four);
  graph->ConnectVertices(4, 2, 0, four);

  graph->ConnectVertices(2, 5, 0, two);
  graph->ConnectVertices(5, 2, 0, two);

  //from and to vertex D
  graph->ConnectVertices(3, 4, 0, three);
  graph->ConnectVertices(4, 3, 0, three);

  //from and to vertex E
  graph->ConnectVertices(4, 5, 0, three);
  graph->ConnectVertices(5, 4, 0, three);

  cout << "Graph Initialized" << endl;
  graph->Print();
  */

  /* currently this is building a graph with same shape as the one in homework 4 */

  //add 6 routers
  for (int i = 0; i < 6; i++)
  {
    AddRouter();
  }

  vector<double> initialEdgeCosts;

  //edges in graph with cost 1
  GenerateEdgeCosts(initialEdgeCosts);
  m_graph.ConnectVertices(0, 3, 0, initialEdgeCosts);
  m_graph.ConnectVertices(3, 0, 0, initialEdgeCosts);
  m_graph.ConnectVertices(1, 2, 0, initialEdgeCosts);
  m_graph.ConnectVertices(2, 1, 0, initialEdgeCosts);

  //edges in graph with cost 2
  GenerateEdgeCosts(initialEdgeCosts);
  m_graph.ConnectVertices(0, 1, 0, initialEdgeCosts);
  m_graph.ConnectVertices(1, 0, 0, initialEdgeCosts);
  m_graph.ConnectVertices(1, 3, 0, initialEdgeCosts);
  m_graph.ConnectVertices(3, 1, 0, initialEdgeCosts);
  m_graph.ConnectVertices(2, 5, 0, initialEdgeCosts);
  m_graph.ConnectVertices(5, 2, 0, initialEdgeCosts);

  //edges in graph with cost 3
  GenerateEdgeCosts(initialEdgeCosts);
  m_graph.ConnectVertices(1, 4, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 1, 0, initialEdgeCosts);
  m_graph.ConnectVertices(3, 4, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 3, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 5, 0, initialEdgeCosts);
  m_graph.ConnectVertices(5, 4, 0, initialEdgeCosts);

  //edges in graph with cost 4
  GenerateEdgeCosts(initialEdgeCosts);
  m_graph.ConnectVertices(2, 4, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 2, 0, initialEdgeCosts);

}

void DynamicRouterNetwork::Update()
{

}

void DynamicRouterNetwork::AddRouter()
{

}

void DynamicRouterNetwork::RemoveRouter(int id)
{

}

void DynamicRouterNetwork::ChangeRouters()
{

}

void DynamicRouterNetwork::GenerateEdgeCosts(vector<double>& edgeCosts)
{
  
}

/**
* For these best path algorithms. The best path can be calculated by recursively
* getting the path cost to each neighboring vertex then traveling to that vertex
* and repeating. To get the path cost, get the edge cost then the neighboring
* node's node cost. Edge cost is calculated by calling GetEdgeCost on the from
* vertex and then passing the id of the to vertex as the parameter to GetEdgeCost(int)
* The node cost can be calculated by calling GetData() on the to vertex and then
* calling GetNodalProcessingDelay() and GetQueueingDelay() on the return val
* from GetData(). And of course if you have any questions please ask me.
*/

vector<int>& DynamicRouterNetwork::BestPathDijsktra(int from, int to)
{
  //yes
}

vector<int>& DynamicRouterNetwork::BestPathBellmanFord(int from, int to)
{
  //my man
}