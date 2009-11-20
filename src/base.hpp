/**
 * @file base.hpp Contains all the necessary header files for creating the
 * program
 */
#ifndef BASE_H_
#define BASE_H_

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif /* __APPLE__ */

using std::ifstream;
using std::string;
using std::vector;
using std::stringstream;

namespace cm0304
{
// Conversion factors for degrees to radians (from wireframe.cc)
static const double deg_to_rad (M_PI / 180.0);
static const double rad_to_deg (180.0 / M_PI);
}

#endif /* BASE_H_ */
