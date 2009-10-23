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

vector_t cross_product(const vector_t& a, const vector_t& b)
{
  vector_t res;
  res.x = (a.y * b.z) - (a.z * b.y);
  res.y = (a.z * b.x) - (a.x * b.z);
  res.z = (a.x * b.y) - (a.y * b.x);
  return res;
}

vector_t find_normal(vector_t& p1, vector_t& p2, vector_t& p3)
{
  vector_t e1 = p2 - p1;
  vector_t e2 = p3 - p1;
  return cross_product(e1, e2);
}
}  // namespace cm0304
