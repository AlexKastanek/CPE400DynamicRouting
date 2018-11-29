#include "Router.h"

Router::Router()
{

}

Router::~Router()
{

}

double Router::GetNodalProcessingDelay()
{
  return m_nodalProcessingDelay;
}

double Router::GetQueuingDelay()
{
  return m_queuingDelay;
}

void Router::SetNodalProcessingDelay(double nodalProcessingDelay)
{
  m_nodalProcessingDelay = nodalProcessingDelay;
}

void Router::SetQueuingDelay(double queuingDelay)
{
  m_queuingDelay = queuingDelay;
}