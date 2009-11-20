/**
 * @file models.hpp Models that can be added to the scene
 */
#ifndef MODELS_H_
#define MODELS_H_

#include "base.hpp"
#include "util.hpp"

namespace cm0304
{
/**
 * Draws a simple quad on the x-z plane that looks something like a floor.
 * @param width Width of the floor in the x direction
 * @param depth Width of the floor in the z direction
 * @param pos y coordinate of the floor
 */
void floor(double width, double depth, double pos);

/**
 * Q.2 (a)
 * Draw a trumpet shaped parametric surface. 
 * Equations are from http://www.math.uri.edu/~bkaskosz/flashmo/tools/parsur/
 * @param res The accuracy with which to draw the parametric surface;
 * the lower, the more accurate.
 */
void parametric_surface(double res);

/**
 * Q.2 (b)
 * Draw a teddy. A call to this function should be made within a glPushMatrix()
 * @param use_vertex_normals If true, each vertex is given a normal. If false,
 * only face normals are used.
 */
void draw_teddy(bool use_vertex_normals);

/**
 * Draw the red teddy
 */
void draw_teddy_one(bool use_vertex_normals);

/**
 * Draw the green teddy
 */
void draw_teddy_two(bool use_vertex_normals);

/** 
 * Number of steam particles to generate. In general, more particles means
 * more steam. It seems to work quite well with a fairly small number of 
 * particles ~ 10 - 20
 */
static const int knum_particles = 20;

/**
 * Data structure for a particle of steam
 */
struct particle_t
{
  double rotation; // Rotation coefficient of the particle
  vertex_t pos; // Position of the bottom of the particle
  double size; // Size of the particle
  double brightness; // Particles fade as they move upwards
  double fade_amount; // Amount the particle should fade
  bool is_dead; // Whether the particle can be renewed
  double speed[3]; // Speed at which the particle moves
};

void init_steam(float spout[3]);
void draw_steam(float spout[3]);
void draw_teapot();
}  // namespace cm0304

#endif /* MODELS_H_ */
