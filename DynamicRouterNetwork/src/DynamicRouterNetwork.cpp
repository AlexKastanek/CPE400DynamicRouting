#include "DynamicRouterNetwork.h"

DynamicRouterNetwork::DynamicRouterNetwork()
{

}

DynamicRouterNetwork::~DynamicRouterNetwork()
{

}

void DynamicRouterNetwork::Initialize()
{

}

void DynamicRouterNetwork::Update()
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