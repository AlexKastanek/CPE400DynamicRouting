#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>

using namespace std;

class Router
{

public:

  //Constructor
  Router();
  Router(double nodalProcessingDelay, double queuingDelay);
  //Destructor
  ~Router();

  double GetNodalProcessingDelay();
  double GetQueuingDelay();

  void SetNodalProcessingDelay(double nodalProcessingDelay);
  void SetQueuingDelay(double queuingDelay);

private:

  double m_nodalProcessingDelay;
  double m_queuingDelay;

};

#endif /* ROUTER_H */