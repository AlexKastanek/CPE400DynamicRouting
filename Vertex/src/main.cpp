#ifndef MAIN
#define MAIN

#include <iostream>

#include "Vertex.hpp"

using namespace std;

int main(void)
{
  Vertex<int>* vertex = new Vertex<int>(3);
  cout << vertex->GetData() << endl;
  delete vertex;

  return 0;
}

#endif /* MAIN */