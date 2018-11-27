#ifndef MAIN
#define MAIN

#include <iostream>

#include "Graph.hpp"

using namespace std;

int main(void)
{
  Graph<int>* graph = new Graph<int>();

  cout << "Building Graph" << endl;

  graph->AddVertex(0, 72);
  graph->AddVertex(1, 53);
  graph->AddVertex(2, 97);

  graph->ConnectVertices(0, 1, 5);
  graph->ConnectVertices(1, 0, 5);

  graph->ConnectVertices(1, 2, 3);
  graph->ConnectVertices(2, 1, 3);

  cout << "Graph Built" << endl;
  graph->Print();

  graph->RemoveVertex(1);

  cout << "Removed Vertex" << endl;
  graph->Print();
  
  graph->Clear();

  cout << "Graph Cleared" << endl;
  graph->Print();

  delete graph;

  return 0;
}

#endif /* MAIN */