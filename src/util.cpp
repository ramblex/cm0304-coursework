/**
 * @file util.cpp Implementation for utility functions
 */
#include "util.hpp"

namespace cm0304
{
double sec(double x)
{
  return (1 / cos(x));
}

vertex_t cross_product(const vertex_t& a, const vertex_t& b)
{
  vertex_t res;
  res.x = (a.y * b.z) - (a.z * b.y);
  res.y = (a.z * b.x) - (a.x * b.z);
  res.z = (a.x * b.y) - (a.y * b.x);
  return res;
}

vertex_t find_normal(vertex_t& p1, vertex_t& p2, vertex_t& p3)
{
  vertex_t e1 = p2 - p1;
  vertex_t e2 = p3 - p1;
  return cross_product(e1, e2);
}
}  // namespace cm0304
