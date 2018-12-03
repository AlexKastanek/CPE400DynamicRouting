#ifndef MAIN
#define MAIN

#include <iostream>

#include "DynamicRouterNetwork.h"

using namespace std;

int main(void)
{

  DynamicRouterNetwork drn;

  drn.Initialize();

  //drn.Print();

  /*
  while (true)
  {
    cout << "UPDATE" << endl;
    drn.Update();
    drn.Print();
  }
  */

  bool running = true;
  int from, to;
  bool validEntry;
  vector<int> *bestPath;

  while (running)
  {
    cout << "Please enter one of the following options:" << endl;
    cout << "1. Step Simulation" << endl
         << "2. Print Network" << endl
         << "3. Dijkstra's" << endl
         << "4. Bellman Ford" << endl
         << "5. Exit" << endl;

    int userSelection;
    cin >> userSelection;

    switch (userSelection)
    {
      case 1:
        drn.Update();
        break;
      case 2:
        drn.Print();
        break;
      case 3:
        validEntry = false;
        while (!validEntry)
        {
          cout << "Please enter the ID of the source router: ";
          cin >> from;
          validEntry = drn.VertexExists(from);
        }

        validEntry = false;
        while (!validEntry)
        {
          cout << "Please enter the ID of the destination router: ";
          cin >> to;
          validEntry = drn.VertexExists(to);
        }

        bestPath = new vector<int>;
        bestPath = drn.BestPathDijsktra(from, to);

        cout << "Best path from " << from << " to " << to << " is { ";
        for (int i = 0; i < bestPath->size(); i++)
        {
          cout << bestPath->at(i) << " ";
        }
        cout << "}" << endl;

        delete bestPath;
        break;
      case 4:
        validEntry = false;
        while (!validEntry)
        {
          cout << "Please enter the ID of the source router: ";
          cin >> from;
          validEntry = drn.VertexExists(from);
        }

        validEntry = false;
        while (!validEntry)
        {
          cout << "Please enter the ID of the destination router: ";
          cin >> to;
          validEntry = drn.VertexExists(to);
        }

        bestPath = new vector<int>;
        bestPath = drn.BestPathBellmanFord(from, to);

        cout << "Best path from " << from << " to " << to << " is { ";
        for (int i = 0; i < bestPath->size(); i++)
        {
          cout << bestPath->at(i) << " ";
        }
        cout << "}" << endl;

        delete bestPath;
        break;
      case 5:
        running = false;
        break;
      default:
        cout << "Invalid entry" << endl;
    }
  }
  
  /*
  cout << "UPDATE" << endl;
  drn.Update();
  drn.Print();
  */

  return 0;
}

#endif /* MAIN */