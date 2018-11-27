#ifndef MAIN
#define MAIN

#include <iostream>

#include "Graph.hpp"

using namespace std;

int main(void)
{
  Graph<char>* graph = new Graph<char>();

  cout << "Building Graph" << endl;

  /* currently this is building the graph in homework 4 */

  graph->AddVertex(0, 'A');
  graph->AddVertex(1, 'B');
  graph->AddVertex(2, 'C');
  graph->AddVertex(3, 'D');
  graph->AddVertex(4, 'E');
  graph->AddVertex(5, 'F');

  //from vertex A
  graph->ConnectVertices(0, 1, 2);
  graph->ConnectVertices(0, 3, 1);

  //from vertex B
  graph->ConnectVertices(1, 0, 2);
  graph->ConnectVertices(1, 2, 1);
  graph->ConnectVertices(1, 3, 2);
  graph->ConnectVertices(1, 4, 3);

  //from vertex C
  graph->ConnectVertices(2, 1, 1);
  graph->ConnectVertices(2, 4, 4);
  graph->ConnectVertices(2, 5, 2);

  //from vertex D
  graph->ConnectVertices(3, 0, 1);
  graph->ConnectVertices(3, 1, 2);
  graph->ConnectVertices(3, 4, 3);

  //from vertex E
  graph->ConnectVertices(4, 1, 3);
  graph->ConnectVertices(4, 2, 4);
  graph->ConnectVertices(4, 3, 3);
  graph->ConnectVertices(4, 5, 3);

  //from vertex F
  graph->ConnectVertices(5, 2, 2);
  graph->ConnectVertices(5, 4, 3);

  cout << "Graph Built" << endl;
  graph->Print();

  graph->RemoveVertex(4);

  cout << "Removed Vertex 4 (E)" << endl;
  graph->Print();
  
  graph->Clear();

  cout << "Graph Cleared" << endl;
  graph->Print();

  delete graph;

  return 0;
}

#endif /* MAIN */