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

  //initialize random seed
  srand(time(NULL));

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
  initialEdgeCosts.clear();

  //edges in graph with cost 2
  GenerateEdgeCosts(initialEdgeCosts);
  m_graph.ConnectVertices(0, 1, 0, initialEdgeCosts);
  m_graph.ConnectVertices(1, 0, 0, initialEdgeCosts);
  m_graph.ConnectVertices(1, 3, 0, initialEdgeCosts);
  m_graph.ConnectVertices(3, 1, 0, initialEdgeCosts);
  m_graph.ConnectVertices(2, 5, 0, initialEdgeCosts);
  m_graph.ConnectVertices(5, 2, 0, initialEdgeCosts);
  initialEdgeCosts.clear();

  //edges in graph with cost 3
  GenerateEdgeCosts(initialEdgeCosts);
  m_graph.ConnectVertices(1, 4, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 1, 0, initialEdgeCosts);
  m_graph.ConnectVertices(3, 4, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 3, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 5, 0, initialEdgeCosts);
  m_graph.ConnectVertices(5, 4, 0, initialEdgeCosts);
  initialEdgeCosts.clear();

  //edges in graph with cost 4
  GenerateEdgeCosts(initialEdgeCosts);
  m_graph.ConnectVertices(2, 4, 0, initialEdgeCosts);
  m_graph.ConnectVertices(4, 2, 0, initialEdgeCosts);
  initialEdgeCosts.clear();

}

void DynamicRouterNetwork::Update()
{
  //initialize random seed
  srand(time(NULL));

  /**
  * generate random number to decide if adding a router, removing a router,
  * or leaving the network alone. Currently doing 50% chance network is left
  * alone, 30% chance a router is added, and 20% chance a router is removed
  */
  int randomNum = rand() % 100 + 1;
  if (randomNum <= 50)
  {
    //do nothing
    cout << "Network state unchanged" << endl;
  }
  else if (randomNum > 80)
  {
    //remove router (randomly generate which router)
    cout << "Removing client from network" << endl;

    //TODO: uncomment when best path algorithms work
    int routerOffset = rand() % (m_graph.GetMap().size() - 1) + 1;
    cout << routerOffset << endl;
    int routerID;
    typename map<int, Vertex<Router>*>::iterator it = m_graph.GetMap().begin();
    for (int i = 0; i < routerOffset; i++)
    {
      it++;
    }
    routerID = it->first;
    RemoveRouter(routerID);
    
  }
  else
  {
    //add router
    AddRouter();
    cout << "Client added to network" << endl;
    cout << "New router index: " << m_currentRouterIndex << endl;

    /* connect router */

    //generate number between 1 and 5
    //this is how many routers the new router is connected to
    int connectionAmount;
    if (m_graph.GetMap().size() > 5)
    {
      connectionAmount = rand() % 5 + 1;
    }
    else
    {
      connectionAmount = rand() % (m_graph.GetMap().size() - 1) + 1;
    }
    
    cout << "Client connecting to " << connectionAmount << " routers..." << endl;

    //generate which routers the new router is connected to
    map<int, bool> connectedRouters;
    for (int i = 0; i < connectionAmount; i++)
    {
      //generate a random router in the network
      int routerOffset = rand() % (m_graph.GetMap().size() - 2);
      typename map<int, Vertex<Router>*>::iterator routerIt = m_graph.GetMap().begin();
      int connectedRouter;
      for (int j = 0; j < routerOffset; j++)
      {
        routerIt++;
      }
      connectedRouter = routerIt->first;
      cout << "Generated router index: " << connectedRouter << endl;

      //check if this router is already connected to new router
      map<int, bool>::iterator it = connectedRouters.find(connectedRouter);
      if (it == connectedRouters.end())
      {
        /*this router is not connected to the new one, connect it*/

        //generate edge costs
        vector<double> edgeCosts;
        GenerateEdgeCosts(edgeCosts);

        cout << "Adding router index: " << connectedRouter << endl;

        //connection must be two way
        m_graph.ConnectVertices(
          m_currentRouterIndex,
          connectedRouter,
          0,
          edgeCosts);
        m_graph.ConnectVertices(
          connectedRouter,
          m_currentRouterIndex,
          0,
          edgeCosts);

        //add this router to the connectedRouters
        connectedRouters[connectedRouter] = true;
      }
      else
      {
        //this router is already connected to the new one
        //increment until a non connected router is discovered
        while (it != connectedRouters.end())
        {
          //get the currently generated router
          int connectedRouterIndex = connectedRouter;
          typename map<int, Vertex<Router>*>::iterator routerIt = m_graph.GetMap().find(connectedRouterIndex);

          //define the iterator in which the router must loop to the beginning of the map
          typename map<int, Vertex<Router>*>::iterator loopPoint = m_graph.GetMap().end();
          loopPoint--;
          loopPoint--;

          //loop back to start if at end, otherwise try the next router
          if (routerIt == loopPoint)
          {
            routerIt = m_graph.GetMap().begin();
          }
          else
          {
            routerIt++;
          }
          connectedRouter = routerIt->first;

          it = connectedRouters.find(connectedRouter);
          //cout << connectedRouter << endl;
        }

        /* connect it */

        //generate edge costs
        vector<double> edgeCosts;
        GenerateEdgeCosts(edgeCosts);

        cout << "Adding router index: " << connectedRouter << endl;

        //connection must be two way
        m_graph.ConnectVertices(
          m_currentRouterIndex,
          connectedRouter,
          0,
          edgeCosts);
        m_graph.ConnectVertices(
          connectedRouter,
          m_currentRouterIndex,
          0,
          edgeCosts);

        //add this router to the connectedRouters
        connectedRouters[connectedRouter] = true;

      }
    }
  }

  //change the router data
  ChangeRouters();
  cout << "Router data updated" << endl;

}

void DynamicRouterNetwork::AddRouter()
{
  double nodalProcessingDelay = 0, queuingDelay = 0;

  //generate random nodal processing delay (value between 0.001 and 0.05 seconds)
  nodalProcessingDelay = ((double)(rand() % 50 + 1)) / 1000.0;
  cout << "Generated nodal processing delay: " << nodalProcessingDelay << endl;

  //generate random queuing delay (value between 0.001 and 0.05 seconds)
  queuingDelay = ((double)(rand() % 50 + 1)) / 1000.0;
  cout << "Generated queuing delay: " << queuingDelay << endl;

  //create router
  Router router(nodalProcessingDelay, queuingDelay);

  //add router to network
  m_currentRouterIndex++;
  m_graph.AddVertex(m_currentRouterIndex, router);
  
}

void DynamicRouterNetwork::RemoveRouter(int id)
{
  if (m_graph.GetMap().size() <= 2)
  {
    cout << "Network is size two and network must contain at least two vertices. Cannot remove router." << endl;
    return;
  }

  cout << "Removing router " << id << endl;
  m_graph.RemoveVertex(id);
}

void DynamicRouterNetwork::ChangeRouters()
{
  //initialize random seed
  srand(time(NULL));

  double changeAmount = 0;

  //loop through each node
  typename map<int, Vertex<Router>*>::iterator it;
  map<int, Vertex<Router>*> routerMap = m_graph.GetMap();
  map<int, bool> changedRouters;
  for (it = routerMap.begin(); it != routerMap.end(); it++)
  {
    //change nodal processing delay
    changeAmount = ((double)(rand() % 100 + 1)) / 10000.0;
    double nodalProcessingDelay = it->second->GetData().GetNodalProcessingDelay();
    nodalProcessingDelay += changeAmount;

    //change queuing delay
    changeAmount = ((double)(rand() % 100 + 1)) / 10000.0;
    double queuingDelay = it->second->GetData().GetQueuingDelay();
    queuingDelay += changeAmount;

    //set the new delays
    Router newRouterData(nodalProcessingDelay, queuingDelay);
    m_graph.SetVertexData(it->first, newRouterData);

    //loop through each neighbor of the node
    Vertex<Router>* vertex = it->second;
    for (int i = 0; i < vertex->m_adjacencyList.size(); i++)
    {
      //only changed the edge cost if this router hasn't already been changed
      map<int, bool>::iterator itChanged = changedRouters.find(vertex->m_adjacencyList[i].second->GetID());
      if (itChanged == changedRouters.end())
      {
        vector<double> edgeDelays;
        Vertex<Router>* neighbor = vertex->m_adjacencyList[i].second;

        //get the transmission and propagation delays
        edgeDelays = vertex->m_adjacencyList[i].first.costs;
        double transmissionDelay = edgeDelays[0];
        double propagationDelay = edgeDelays[1];

        //change transmission delay
        changeAmount = ((double)(rand() % 100 + 1)) / 10000.0;
        transmissionDelay += changeAmount;

        //change propagation delay
        changeAmount = ((double)(rand() % 100 + 1)) / 10000.0;
        propagationDelay += changeAmount;

        //set the new delays
        edgeDelays[0] = transmissionDelay;
        edgeDelays[1] = propagationDelay;
        m_graph.SetVertexEdgeCosts(
          it->first, 
          neighbor->GetID(),
          edgeDelays);
        m_graph.SetVertexEdgeCosts(
          neighbor->GetID(), 
          it->first,
          edgeDelays);
      }
    }

    //add this router to the list of changed routers
    changedRouters[it->first] = true;
  }
}

void DynamicRouterNetwork::GenerateEdgeCosts(vector<double>& edgeCosts)
{
  double transmissionDelay = 0, propagationDelay = 0;

  //generate random transmission delay (value between 0.0001 and 0.05 seconds)
  transmissionDelay = ((double)(rand() % 500 + 1)) / 10000.0;
  cout << "Generated transmission delay: " << transmissionDelay << endl;

  //generate random propagation delay (value between 0.000001 and 0.001)
  //this value will be a lot larger for wireless links
  propagationDelay = ((double)(rand() % 1000 + 1)) / 1000000.0;
  cout << "Generated propagation delay: " << propagationDelay << endl;

  //set the delays
  edgeCosts.push_back(transmissionDelay);
  edgeCosts.push_back(propagationDelay);
}

bool DynamicRouterNetwork::VertexExists(int id)
{
  typename map<int, Vertex<Router>*>::iterator it = m_graph.GetMap().find(id);
  
  //if id is in the map then the vertex exists
  if (it != m_graph.GetMap().end())
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool DynamicRouterNetwork::PathExists(int from, int to)
{
  return m_graph.BFS(from, to);
}

void DynamicRouterNetwork::Print()
{
  m_graph.Print();
}

vector<int>* DynamicRouterNetwork::BestPathDijsktra(int from, int to)
{
  int verticesRemaining = m_graph.GetMap().size();
  map<int, bool> inSet; //used to create a set of unvisted nodes
  map<int, double> distance; //creates a set of shortest lengths to each node
  map<int, int> previous; //keeps track of the previous node on the path to a given node. Ex. previous[3] = 5 means that 3 was reached through 5 on the shortest path

  //initialize maps
  typename map<int, Vertex<Router>*>::iterator it;
  for (it = m_graph.GetMap().begin(); it != m_graph.GetMap().end(); it++)
  {
    inSet[it->first] = true; //true = node has not been visited yet, 
    distance[it->first] = 99999.0; //99999.0=inf
    previous[it->first] = -1;
  }

  distance [from] = 0; //Source -> Source distance = 0

  while (verticesRemaining > 0)
  {
    Vertex<Router>* nextVertex = NULL;
    double shortestDistance = 99999.0;
    int nextVertexIndex = -1;

    //Find the next shortest distance node u
    for (it = m_graph.GetMap().begin(); it != m_graph.GetMap().end(); it++)
    {
      if (distance[it->first] <= shortestDistance && inSet[it->first]) //If shortest distance and has not been visited yet
      {
        nextVertex = it->second;
        shortestDistance = distance[it->first];
        nextVertexIndex = it->first;
      }
    }

    inSet [nextVertexIndex] = false; //Remove node u from the set

    if (nextVertexIndex == to) //If we have found the destination we can terminate the search
    {
      break;
    }

    //Find shorter paths from this node
    for (int i = 0; i < nextVertex->m_adjacencyList.size(); i++)
    {
      //add up each componenet of the vertex's edge cost
      double alternatePathCost = 0;
      vector<double> edgeCosts = nextVertex->m_adjacencyList[i].first.costs;
      alternatePathCost = edgeCosts[0] + edgeCosts[1] +
        nextVertex->m_adjacencyList[i].second->GetData().GetNodalProcessingDelay() +
        nextVertex->m_adjacencyList[i].second->GetData().GetQueuingDelay();

      //add path so far
      alternatePathCost += distance [nextVertexIndex];

      int destinationIndex = nextVertex->m_adjacencyList[i].second->GetID(); //The index of node v

      //If a shorter path was found from u -> v, where v has not been visted yet
      if (alternatePathCost < distance [destinationIndex] && inSet [destinationIndex])
      {
        distance [destinationIndex] = alternatePathCost;
        previous [destinationIndex] = nextVertexIndex;
      }
    }

    verticesRemaining--;
  }

  stack <int> reversePath;

  int localDestination = to;
  int localSource = -1;

  reversePath.push (localDestination);

  do
  {
    localSource = previous [localDestination];

    reversePath.push (localSource);

    localDestination = localSource;

  }
  while (localSource != from);

  vector <int>* path = new vector<int>;

  while (!reversePath.empty())
  {
    path->push_back (reversePath.top ());
    reversePath.pop ();
  }

  /*
  //This can be removed, it is just debug output
  cout << "Best DJ path from " << from << " to " << to << " is { ";
  for (int i = 0; i < path->size (); i++)
  {
    cout << path->at(i) << " ";

  }
  cout << "}" << endl;
  */

  return path;

}

vector<int>* DynamicRouterNetwork::BestPathBellmanFord(int from, int to)
{
  int vertexCount = m_graph.GetMap().size();
  map<int, double> distance; //creates a set of shortest lengths to each node
  map<int, int> previous; //keeps track of the previous node on the path to a given node. Ex. previous[3] = 5 means that 3 was reached through 5 on the shortest path

  //initialize maps
  typename map<int, Vertex<Router>*>::iterator it;
  for (it = m_graph.GetMap().begin(); it != m_graph.GetMap().end(); it++)
  {
    distance[it->first] = 99999.0; //99999.0=inf
    previous[it->first] = -1;
  }

  distance [from] = 0; //Set Source -> Source distance to 0

  for (it = m_graph.GetMap().begin(); it != m_graph.GetMap().end(); it++)
  {
    //For each edge (u, v)
    map<int, Vertex<Router>*>::iterator it2;
    for (it2 = it; it2 != m_graph.GetMap().end(); it2++)
    {
      Vertex<Router>* nextVertex = it2->second;
      for (int i = 0; i < nextVertex->m_adjacencyList.size(); i++)
      {
        int destinationIndex = nextVertex->m_adjacencyList[i].second->GetID();
    
        //Calculate weight
        double weight;
        vector<double> edgeCosts = nextVertex->m_adjacencyList[i].first.costs;
        weight = edgeCosts[0] + edgeCosts[1] +
          nextVertex->m_adjacencyList[i].second->GetData().GetNodalProcessingDelay() +
          nextVertex->m_adjacencyList[i].second->GetData().GetQueuingDelay();
    
        if ((weight + distance[it2->first]) < distance[destinationIndex])
        {
          distance [destinationIndex] = weight + distance[it2->first];
          previous [destinationIndex] = it2->first;
        }
      }
    }
  }

  //Here checking for negative-weight cycles would be done, but since we are guarenteed to not have any it can be skipped

  stack <int> reversePath;

  int localDestination = to;
  int localSource = -1;

  reversePath.push (localDestination);

  do
  {
    localSource = previous [localDestination];

    reversePath.push (localSource);

    localDestination = localSource;

  }
  while (localSource != from);

  vector <int>* path = new vector<int>;

  while (!reversePath.empty())
  {
    path->push_back (reversePath.top ());

    reversePath.pop ();

  }

  /*
  //This can be removed, it is just debug output
  cout << "Best BF path from " << from << " to " << to << " is { ";
  for (int i = 0; i < path->size (); i++)
  {
    cout << path->at(i) << " ";

  }
  cout << "}" << endl;
  */

  return path;

}

Graph<Router>& DynamicRouterNetwork::GetGraph()
{
  return m_graph;
}