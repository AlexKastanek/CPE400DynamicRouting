#ifndef MAIN
#define MAIN

#include <iostream>

#include "Vertex.hpp"

using namespace std;

int main(void)
{
  Vertex<int>* vertex = new Vertex<int>(0, 3);
  
  cout << "Vertex " << vertex->GetID() << ": " << vertex->GetData() << endl;

  delete vertex;

  return 0;
}

#endif /* MAIN */