#ifndef MAIN
#define MAIN

#include <iostream>

#include "Graph.hpp"

using namespace std;

int main(void)
{
  Graph<int>* graph = new Graph<int>();

  graph->AddVertex(0, 72);
  graph->AddVertex(1, 53);
  graph->AddVertex(2, 97);

  graph->ConnectVertices(0, 1, 5);
  graph->ConnectVertices(1, 0, 5);

  graph->ConnectVertices(1, 2, 3);
  graph->ConnectVertices(2, 1, 3);

  graph->Print();

  delete graph;

  return 0;
}

#endif /* MAIN */