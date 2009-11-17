/**
 * @file models.hpp Models that can be added to the scene
 */
#ifndef MODELS_H_
#define MODELS_H_

#include "base.hpp"

namespace cm0304
{
/**
 * Draw teddy.ply using vertex normals rather than just face normals. This
 * gives a smoother look.
 */
void vertex_normals_teddy();

void subdivision_teddy();

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
