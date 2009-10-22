/**
 * @file coursework.cpp
 * CM0304 coursework 2009
 *
 * Implements a basic 3D OpenGL scene which can be navigated in a walk-through
 * fashion.
 *
 * Copyright 2009 Alex Duller
 */
#include <iostream>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif /* __APPLE__ */

// Conversion factors for degrees to radians (from wireframe.cc)
const double deg_to_rad (M_PI / 180.0);
const double rad_to_deg (180.0 / M_PI);

// ID of display list containing scene description
GLuint scene_dl(0U);

// The y rotation of the camera in degrees
float angle = 0;

// Rate at which camera is rotated, higher means faster rotation
float rotate_sensitivity = 0.4;

float dist_sensitivity = 0.1;

// The inverted position of the camera
float position[3]={0, -2, -30};

// Used to keep track of keyboard keys - this is so we can use multiple keys
// at once.
int g_keys[128] = {false};

// Handle special keys when they are pressed (idea from ...)
void special_callback(int key, int, int)
{
  g_keys[key] = true;
}

// Unset the special key flag.
void special_up_callback(int key, int, int)
{
  g_keys[key] = false;
}

/**
 * Q2. 3D objects
 * A parametric surface which is not a plane, sphere, cylinder or cone. This
 * can be added to the scene through the scene() function.
 *
 * A parametric surface is defined by parametric functions:
 * x = fx(u, v)
 * y = fy(u, v)
 * y = fz(u, v)
 *
 * @param accuracy The approximation accuracy of the surface by polygons
 */
void cylinder()
{
  // Surface material
  static GLfloat diffuse[] = {0.7, 0.0, 0.0, 1.0};
  static GLfloat ambient[] = {0.5, 0.0, 0.0, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  glPushMatrix();
  glTranslated(0.0, 1.0, 0.0);

  int r = 1;

  for (double h = 0; h < 5; h += 0.1)
  {
    glBegin(GL_QUAD_STRIP);
    for (double theta = -180; theta <= 180; theta += 0.1)
    {
      double rad_theta = deg_to_rad * theta;
      double x = r * cos(rad_theta);
      double y = r * sin(rad_theta);
      double z = h;
      glVertex3f(x, z, y);

      rad_theta = deg_to_rad * (theta + 0.1);
       x = r * cos(rad_theta);
       y = r * sin(rad_theta);
       z = h + 0.1;
      glVertex3f(x, z, y);
    }
    glEnd();
  }

  glPopMatrix();
  return;
}

/**
 * Q2. 3D objects
 * Stanford bunny mesh
 */
void bunny()
{

}

// Sphere from shaded.cc
void sphere()
{
  // Angle steps for facet size
  static double da (1.0);

  // Sphere material
  static GLfloat diffuse[] = {0.7, 0.0, 0.0, 1.0};
  static GLfloat ambient[] = {0.5, 0.0, 0.0, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  glPushMatrix();
  glTranslated(0.0, 1.0, 0.0);

  // For shading we need surface normals in addition to vertex
  // positions; for the unit sphere the normal is the same than the
  // surface position (note that this is a very special case!).
  //
  // We first set the normal of the vertex and then the vertex itself.

  // Quadrilateral strips
  for (double phi = -90.0 + da; phi < 90.0; phi += da) {
    glBegin (GL_QUAD_STRIP);
    for (double thet = -180.0; thet <= 180.0; thet += da) {
      double u = sin (deg_to_rad * thet) * cos (deg_to_rad * phi);
      glNormal3d (u,
		  cos (deg_to_rad * thet) * cos (deg_to_rad * phi),
		  sin (deg_to_rad * phi));
      glVertex3d (sin (deg_to_rad * thet) * cos (deg_to_rad * phi),
		  cos (deg_to_rad * thet) * cos (deg_to_rad * phi),
		  sin (deg_to_rad * phi));
      glNormal3d (sin (deg_to_rad * thet) * cos (deg_to_rad * (phi + da)),
		  cos (deg_to_rad * thet) * cos (deg_to_rad * (phi + da)),
		  sin (deg_to_rad * (phi + da)));
      glVertex3d (sin (deg_to_rad * thet) * cos (deg_to_rad * (phi + da)),
		  cos (deg_to_rad * thet) * cos (deg_to_rad * (phi + da)),
		  sin (deg_to_rad * (phi + da)));
    }
    glEnd();
  }

  // North pole
  glBegin (GL_TRIANGLE_FAN);
  glNormal3d (0.0, 0.0, 1.0);
  glVertex3d (0.0, 0.0, 1.0);
  for (double thet = -180.0; thet <= 180.0; thet += 10.0) {
    glNormal3d (sin (deg_to_rad * thet) * cos (deg_to_rad * (90.0 - da)),
		cos (deg_to_rad * thet) * cos (deg_to_rad * (90.0 - da)),
		sin (deg_to_rad * 80.0));
    glVertex3d (sin (deg_to_rad * thet) * cos (deg_to_rad * (90.0 - da)),
		cos (deg_to_rad * thet) * cos (deg_to_rad * (90.0 - da)),
		sin (deg_to_rad * 80.0));
  }
  glEnd();

  // South pole
  glBegin (GL_TRIANGLE_FAN);
  glNormal3d (0.0, 0.0, -1.0);
  glVertex3d (0.0, 0.0, -1.0);
  for (double thet = -180.0; thet <= 180.0; thet += 10.0) {
    glNormal3d (sin (deg_to_rad * thet) * cos (deg_to_rad * (-90.0 + da)),
		cos (deg_to_rad * thet) * cos (deg_to_rad * (-90.0 + da)),
		sin (deg_to_rad * (-90.0 + da)));
    glVertex3d (sin (deg_to_rad * thet) * cos (deg_to_rad * (-90.0 + da)),
		cos (deg_to_rad * thet) * cos (deg_to_rad * (-90.0 + da)),
		sin (deg_to_rad * (-90.0 + da)));
  }
  glEnd();
  glPopMatrix();

  return;
}

// Draw the floor of the scene
void floor()
{
  static GLfloat diffuse[] = {0.1, 0.1, 0.0, 1.0};
  static GLfloat ambient[] = {0.1, 0.5, 0.5, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine(3.0);

  // Set material
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shine);

  // Push current modelview matrix on a matrix stack to save current
  // transformation.
  glPushMatrix ();

  glTranslated (0.0, 0.0, 0.0);
  glRotated (0.0, 1.0, 0.0, 0.0);

  glBegin(GL_LINES);
  for (int i = -40; i <= 40; ++i)
  {
    glVertex3f(i, 0, -40);
    glVertex3f(i, 0, 40);
    glVertex3f(-40, 0, i);
    glVertex3f(40, 0, i);
  }
  glEnd();

  // Get original matrix back from stack (undo above transformation
  // for objects drawn after this one)
  glPopMatrix ();
}

/**
 * Helper method to work out the secant of a given angle
 * @param x The angle
 * @return The secant of the angle, x
 */
static double sec(double x)
{
  return (1 / cos(x));
}

/**
 * Draw a hyperbolic paraboloid.
 * The parametric equations for the hyperbolic paraboloid are from:
 * http://www.physicsforums.com/showthread.php?t=89217
 * @param res The accuracy with which to draw the parametric surface; 
 * the lower, the more accurate.
 */
void parametric_surface(double res)
{
  // Define material
  static GLfloat diffuse[] = {0.1, 0.9, 0.0, 1.0};
  static GLfloat ambient[] = {0.5, 0.5, 0.0, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (3.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  // Push current modelview matrix on a matrix stack to save current
  // transformation.
  glPushMatrix ();

  glTranslated (3.0, -0.9, 0.0);
  glRotated (0.0, 1.0, 0.0, 0.0);

  for (double s = -180/4; s <= 180/4; s += res)
  {
    glBegin(GL_QUAD_STRIP);
    for (double t = -180/4; t <= 180/4; t += res)
    {
      double u = deg_to_rad * s;
      double v = deg_to_rad * t;
      glVertex3f(2 * tan(v) * sec(u), sqrt(2) * sec(u) * sec(v), 3 * tan(u));

      u = deg_to_rad * (s + res);
      v = deg_to_rad * (t + res);
      glVertex3f(2 * tan(v) * sec(u), sqrt(2) * sec(u) * sec(v), 3 * tan(u));
    }
    glEnd();
  }

  // Get original matrix back from stack (undo above transformation
  // for objects drawn after this one)
  glPopMatrix ();
}

/**
 * Add objects to the scene. Order does not matter since we are using
 * DEPTH_TEST. They will be automatically added to the scene's display list
 */
void scene()
{
  floor();

  // For Q2. (a)
  parametric_surface(0.3);

  return;
}

/**
 * Initialise the lights for the scene. Adapted from shaded.cc
 */
void init_lights()
{
  glEnable(GL_LIGHTING);

  glPolygonMode(GL_FRONT, GL_FILL);

  // Treat polygon front and back side equal (no two-sided polygons)
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  // Enable local viewer model for specular light
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  // Use Gourad-shading technique
  glShadeModel(GL_SMOOTH);

  {
    // Light source 0
    static GLfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    static GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    static GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};

    // Enable first light source
    glEnable (GL_LIGHT0);
    // Set light emitted by light source 0
    glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv (GL_LIGHT0, GL_SPECULAR, specular);
  }

  return;
}

// Set position of light sources
void lights()
{
  static GLfloat light0_pos[] = {0.0f, 3.0f, 2.0f, 1.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

  return;
}

// Render the scene
void display_callback(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Camera position
  glRotatef(angle, 0, 1, 0);
  glTranslatef(position[0], position[1], position[2]);

  // Add light to the scene
  lights();

  // Render the scene
  glCallList(scene_dl);

  glFlush();

  // Switch to back buffer since we are using double buffering
  glutSwapBuffers();
}

// Called when window is resized (from ...)
void reshape_callback(int w, int h)
{
  if (w == 0)
    h = 1;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, (float)w/h, 0.1, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/**
 * Handle keyboard input. Idea adapted from ()
 * Q1 (b) Navigation system
 */
void idle_callback()
{
  if (g_keys[GLUT_KEY_LEFT])
    angle -= rotate_sensitivity;

  if (g_keys[GLUT_KEY_RIGHT])
    angle += rotate_sensitivity;

  if (g_keys[GLUT_KEY_UP])
  {
    position[0] += -sin(angle * M_PI / 180.0f) * dist_sensitivity;
    position[2] += cos(angle * M_PI / 180.0f) * dist_sensitivity;
  }

  if (g_keys[GLUT_KEY_DOWN])
  {
    position[0] -= -sin(angle * M_PI / 180.0f) * dist_sensitivity;
    position[2] -= cos(angle * M_PI / 180.0f) * dist_sensitivity;
  }

  // Q1. (a) continuously render scene
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Grid");

  // Enable visible surface detection via depth tests
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  // Setup callback functions
  glutDisplayFunc(display_callback);
  glutReshapeFunc(reshape_callback);
  glutSpecialFunc(special_callback);
  glutSpecialUpFunc(special_up_callback);
  glutIdleFunc(idle_callback);

  // Initialise lights so that they are ready to be added to the scene
  init_lights();

  // Create a display list for the scene
  scene_dl = glGenLists(1);
  glNewList(scene_dl, GL_COMPILE);
  scene();
  glEndList();

  glutMainLoop();

  return 0;
}
