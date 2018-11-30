#include "Router.h"

Router::Router()
{
  m_nodalProcessingDelay = 0;
  m_queuingDelay = 0;
}

Router::Router(double nodalProcessingDelay, double queuingDelay)
{
  m_nodalProcessingDelay = nodalProcessingDelay;
  m_queuingDelay = queuingDelay;
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