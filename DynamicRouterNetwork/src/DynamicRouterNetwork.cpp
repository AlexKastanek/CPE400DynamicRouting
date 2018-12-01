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
  * or leaving the network alone. Currently doing 85% chance network is left
  * alone, 10% chance a router is added, and 5% chance a router is removed
  */
  int randomNum = rand() % 100 + 1;
  if (randomNum <= 10)
  {
    //do nothing
    cout << "Network state unchanged" << endl;
  }
  else if (randomNum > 95)
  {
    //remove router (randomly generate which router)
    cout << "Client removed from network" << endl;
  }
  else
  {
    //add router
    AddRouter();
    cout << "Client added to network" << endl;
    cout << "New router index: " << m_currentRouterIndex << endl;

    /* connect router */

    //generate number between 1 network size
    //this is how many routers the new router is connected to
    int connectionAmount = rand() % m_graph.GetMap().size() + 1;
    cout << "Client connecting to " << connectionAmount << " routers..." << endl;

    //generate which routers the new router is connected to
    map<int, bool> connectedRouters;
    for (int i = 0; i < connectionAmount; i++)
    {
      //generate a random router in the network
      int connectedRouter = rand() % (m_graph.GetMap().size() - 1);

      //check if this router is already connected to new router
      map<int, bool>::iterator it = connectedRouters.find(connectedRouter);
      if (it == connectedRouters.end())
      {
        /*this router is not connected to the new one, connect it*/

        //generate edge costs
        vector<double> edgeCosts;
        GenerateEdgeCosts(edgeCosts);

        cout << "This router index: " << connectedRouter << endl;

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
        while (it == connectedRouters.end())
        {
          int connectedRouterIndex = connectedRouter + 1;
          connectedRouter = connectedRouterIndex % m_graph.GetMap().size();
          map<int, bool>::iterator it = connectedRouters.find(connectedRouter);
        }

        /* connect it */

        //generate edge costs
        vector<double> edgeCosts;
        GenerateEdgeCosts(edgeCosts);

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

void DynamicRouterNetwork::Print()
{
  m_graph.Print();
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