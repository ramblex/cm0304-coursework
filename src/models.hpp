/**
 * @file models.hpp Models that can be added to the scene
 */
#ifndef MODELS_H_
#define MODELS_H_

#include "base.hpp"
#include "util.hpp"

namespace cm0304
{
class Model
{
 public:
  virtual void draw() = 0;
};

class Teddy : public Model
{
 public:
  Teddy(int num_subdivides) 
      : m_read_mesh(false), m_num_subdivides(num_subdivides)
  {}

  void draw();
 private:
  void read_mesh();
  void set_material();
  void subdivide();

  bool m_read_mesh;
  int m_num_subdivides; // Number of times to apply subdivision
  vector<triangle_t> m_faces;
  vector<cm_vertex_t> m_vertices;
  static const int m_num_vertices = 202;
  static const int m_num_faces = 400;
};

/**
 * Draw teddy.ply using vertex normals rather than just face normals. This
 * gives a smoother look.
 */
void vertex_normals_teddy();

/**
 * Reads the mesh from teddy.ply and draws it on the screen. It calculates
 * the normals of each surface and so can be used with GL_SMOOTH.
 */
//void teddy(bool smooth);


/**
 * Draw a trumpet shaped parametric surface. 
 * Equations are from http://www.math.uri.edu/~bkaskosz/flashmo/tools/parsur/
 * @param res The accuracy with which to draw the parametric surface;
 * the lower, the more accurate.
 */
void parametric_surface(double res);

/**
 * Draws a simple grid that looks something like a floor.
 */
void floor();
}  // namespace cm0304

#endif /* MODELS_H_ */
