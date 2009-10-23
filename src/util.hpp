/**
 * @file util.hpp This file contains all the utility functions which are useful
 * when drawing objects in OpenGL.
 */
#ifndef UTIL_H_
#define UTIL_H_

#include "base.hpp"

namespace cm0304
{
/**
 * Model a vector in a structure. This allows us to easily perform operations
 * on vectors without having to do lots of mangling of arrays.
 */
struct vector_t
{
  double x;
  double y;
  double z;
};

/**
 * Helper method to work out the secant of a given angle
 * @param x The angle
 * @return The secant of the angle, x
 */
double sec(double x);

/**
 * Substract two vectors and return the resulting vector
 */
inline vector_t operator-(const vector_t& v1, const vector_t& v2)
{
  vector_t res;
  res.x = v1.x - v2.x;
  res.y = v1.y - v2.y;
  res.z = v1.z - v2.z;
  return res;
}

/**
 * Calculate the cross product of two given vectors and return the result
 */
vector_t cross_product(const vector_t& a, const vector_t& b);

/**
 * Find the normal of three vectors. This can be used to calculate the normal
 * of a triangle face.
 */
vector_t find_normal(vector_t& p1, vector_t& p2, vector_t& p3);
}  // namespace cm0304

#endif /* UTIL_H_ */
