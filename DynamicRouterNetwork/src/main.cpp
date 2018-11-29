#ifndef MAIN
#define MAIN

#include <iostream>

#include "Graph.hpp"

using namespace std;

int main(void)
{
  Graph<char>* graph = new Graph<char>();

  //cout << "Building Graph" << endl;

  /* currently this is building the graph in homework 4 */

  //initialize single vectors with one value
  vector<double> one(1, 1);
  vector<double> two(1, 2);
  vector<double> three(1, 3);
  vector<double> four(1, 4);

  //initialize edge data, one for each vector
  EdgeData edgeOne(0, one);
  EdgeData edgeTwo(0, two);
  EdgeData edgeThree(0, three);
  EdgeData edgeFour(0, four);

  graph->AddVertex(0, 'A');
  graph->AddVertex(1, 'B');
  graph->AddVertex(2, 'C');
  graph->AddVertex(3, 'D');
  graph->AddVertex(4, 'E');
  graph->AddVertex(5, 'F');

  //from vertex A
  graph->ConnectVertices(0, 1, 0, two);
  graph->ConnectVertices(0, 3, 0, one);

  //from vertex B
  graph->ConnectVertices(1, 0, 0, two);
  graph->ConnectVertices(1, 2, 0, one);
  graph->ConnectVertices(1, 3, 0, two);
  graph->ConnectVertices(1, 4, 0, three);

  //from vertex C
  graph->ConnectVertices(2, 1, 0, one);
  graph->ConnectVertices(2, 4, 0, four);
  graph->ConnectVertices(2, 5, 0, two);

  //from vertex D
  graph->ConnectVertices(3, 0, 0, one);
  graph->ConnectVertices(3, 1, 0, two);
  graph->ConnectVertices(3, 4, 0, three);

  //from vertex E
  graph->ConnectVertices(4, 1, 0, three);
  graph->ConnectVertices(4, 2, 0, four);
  graph->ConnectVertices(4, 3, 0, three);
  graph->ConnectVertices(4, 5, 0, three);

  //from vertex F
  graph->ConnectVertices(5, 2, 0, two);
  graph->ConnectVertices(5, 4, 0, three);

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