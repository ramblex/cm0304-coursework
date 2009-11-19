/**
 * @file util.hpp This file contains all the utility functions which are useful
 * when drawing objects in OpenGL. The data structures in this file are adapted
 * from exercise G-II solutions.
 */
#ifndef UTIL_H_
#define UTIL_H_

#include "base.hpp"
#include <iostream>
#include <map>
#include <set>

namespace cm0304
{
using std::ostream;
using std::map;
using std::set;

/**
 * Model a vector in a structure. This allows us to easily perform operations
 * on vectors without having to do lots of mangling of arrays.
 */
struct vertex_t
{
  double x;
  double y;
  double z;

  vertex_t* normal;

  vertex_t () : x(0), y(0), z(0) {}
  vertex_t (double _x, double _y, double _z) 
      : x(_x), y(_y), z(_z) {}
};

struct triangle_t
{
  int v1_idx;
  int v2_idx;
  int v3_idx;
  vertex_t normal;
};

/**
 * Helper method to work out the secant of a given angle
 * @param x The angle
 * @return The secant of the angle, x
 */
double sec(double x);


/**
 * Substract two vertices and return the resulting vertex
 */
inline vertex_t operator-(const vertex_t& v1, const vertex_t& v2)
{
  vertex_t res;
  res.x = v1.x - v2.x;
  res.y = v1.y - v2.y;
  res.z = v1.z - v2.z;
  return res;
}

// Add two vertices and return the resulting vertex
inline vertex_t operator+(const vertex_t& v1, const vertex_t& v2)
{
  vertex_t res;
  res.x = v1.x + v2.x;
  res.y = v1.y + v2.y;
  res.z = v1.z + v2.z;
  return res;
}

// Divide a vertex by a scalar and return the resulting vertex
inline vertex_t operator/(const vertex_t& v1, const double i)
{
  vertex_t res;
  res.x = v1.x / i;
  res.y = v1.y / i;
  res.z = v1.z / i;
  return res;
}

inline vertex_t operator*(const vertex_t& a, const vertex_t& b)
{
  vertex_t res;
  res.x = a.x * b.x;
  res.y = a.y * b.y;
  res.z = a.z * b.z;
  return res;
}

inline vertex_t operator*(const vertex_t& v1, const int i)
{
  vertex_t res;
  res.x = v1.x * i;
  res.y = v1.y * i;
  res.z = v1.z * i;
  return res;
}

inline vertex_t& operator+=(vertex_t& a, const vertex_t& b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

inline ostream& operator<<(ostream& os, const vertex_t& v)
{
  return os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

inline bool operator==(const vertex_t& a, const vertex_t& b)
{
  return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

inline bool operator<(const vertex_t& a, const vertex_t& b)
{
  return ((a.x + a.y + a.z) < (b.x + b.y + b.z));
}

/**
 * Calculate the cross product of two given vectors and return the result
 */
vertex_t cross_product(const vertex_t& a, const vertex_t& b);

/**
 * Find the normal of three vectors. This can be used to calculate the normal
 * of a triangle face.
 */
vertex_t find_normal(vertex_t& v1, vertex_t& v2, vertex_t& v3);
}  // namespace cm0304

#endif /* UTIL_H_ */
