/**
 * @file models.hpp Models that can be added to the scene
 */
#ifndef MODELS_H_
#define MODELS_H_

#include "base.hpp"

/**
 * Reads the mesh from teddy.ply and draws it on the screen. It calculates
 * the normals of each surface and so can be used with GL_SMOOTH.
 */
void teddy();

/**
 * Draws a hyperbolic paraboloid and calculates the normals for each quad.
 */
void parametric_surface(double res);

/**
 * Draws a simple grid that looks something like a floor.
 */
void floor();

#endif /* MODELS_H_ */
