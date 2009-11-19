/**
 * @file models.hpp Models that can be added to the scene
 */
#ifndef MODELS_H_
#define MODELS_H_

#include "base.hpp"
#include "util.hpp"

namespace cm0304
{
void init_steam(float spout[3]);
void draw_steam(float spout[3]);

void draw_teapot();

void draw_teddy_one(bool use_vertex_normals);
void draw_teddy_two(bool use_vertex_normals);
void draw_teddy(bool use_vertex_normals);

void draw_mesh();

/**
 * Read in a PLY mesh into m_faces and m_vertices
 */
void read_mesh(const string& filename, int vertices, int faces);

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
void floor(double width, double depth, double pos);
}  // namespace cm0304

#endif /* MODELS_H_ */
