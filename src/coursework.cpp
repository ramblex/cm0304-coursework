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
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using std::ifstream;
using std::string;
using std::vector;
using std::stringstream;

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

struct vector_t
{
  double x;
  double y;
  double z;
};

inline vector_t operator-(const vector_t& v1, const vector_t& v2)
{
  vector_t res;
  res.x = v1.x - v2.x;
  res.y = v1.y - v2.y;
  res.z = v1.z - v2.z;
  return res;
}

inline vector_t cross_product(const vector_t& a, const vector_t& b)
{
  vector_t res;
  res.x = (a.y * b.z) - (a.z * b.y);
  res.y = (a.z * b.x) - (a.x * b.z);
  res.z = (a.x * b.y) - (a.y * b.x);
  return res;
}

vector_t find_normal(vector_t& p1, vector_t& p2, vector_t& p3)
{
  vector_t e1 = p3 - p2;
  vector_t e2 = p2 - p1;
  return cross_product(e1, e2);
}

/**
 * Q2. 3D objects
 * Stanford bunny mesh
 */
void teddy()
{
  // Material
  static GLfloat diffuse[] = {0.7, 0.0, 0.0, 1.0};
  static GLfloat ambient[] = {0.5, 0.0, 0.0, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  ifstream is("teddy.ply");
  string line = "";
  const int num_vertices = 202; // Number of vertices in the mesh
  const int num_faces = 400; // Number of faces in the mesh
  // Parse the header
  while (getline(is, line))
  {
    if (line == "end_header")
      break;
  }

  // Read in the vertices
  vector<vector_t> vertices;
  int num_vertices_read = 0;
  while (num_vertices_read < num_vertices)
  {
    getline(is, line);
    stringstream ss(line);
    vector_t v;
    ss >> v.x >> v.y >> v.z;
    vertices.push_back(v);
    ++num_vertices_read;
  }

  glPushMatrix();
  glEnable(GL_NORMALIZE);
  glScalef(0.2, 0.2, 0.2);
  glTranslated(0.0, 20.0, 0.0);
  // Read in the faces
  int num_faces_read = 0;
  glBegin(GL_TRIANGLES);
  while (num_faces_read < num_faces)
  {
    getline(is, line);
    stringstream ss(line);
    int num = 0; // Number of vertices - this is discarded
    int v1 = -1;
    int v2 = -1;
    int v3 = -1;
    ss >> num >> v1 >> v2 >> v3;
    // Assume that the vertices of the triangle are given in an anti-clockwise
    // direction.
    vector_t n = find_normal(vertices[v1], vertices[v2], vertices[v3]);
    glNormal3f(n.x, n.y, n.z);
    glVertex3f(vertices[v1].x, vertices[v1].y, vertices[v1].z);
    glNormal3f(n.x, n.y, n.z);
    glVertex3f(vertices[v2].x, vertices[v2].y, vertices[v2].z);
    glNormal3f(n.x, n.y, n.z);
    glVertex3f(vertices[v3].x, vertices[v3].y, vertices[v3].z);
    ++num_faces_read;
  }
    glEnd();
    glPopMatrix();
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
  glEnable(GL_NORMALIZE);
  glTranslated (20.0, -0.9, 0.0);
  glRotated (0.0, 1.0, 0.0, 0.0);

  for (double s = -180/4; s <= 180/4; s += res)
  {
    glBegin(GL_QUAD_STRIP);
    for (double t = -180/4; t <= 180/4; t += res)
    {
      double u = deg_to_rad * s;
      double v = deg_to_rad * t;
      double x1 = 2 * tan(v) * sec(u);
      double y1 = sqrt(2) * sec(u) * sec(v);
      glVertex3f(x1, y1, 3 * tan(u));

      u = deg_to_rad * (s + res);
      v = deg_to_rad * (t + res);
      double x2 = 2 * tan(v) * sec(u);
      double y2 = sqrt(2) * sec(u) * sec(v);
      glVertex3f(x2, y2, 3 * tan(u));

      // Work out the normal for the face
      // Adapted from:
      // http://www.opengl.org/resources/code/samples/glut_examples/examples/dinoshade.c
      double dx = y2 - y1;
      double dy = x2 - x1;
      double len = sqrt((dx * dx) + (dy * dy));
      glNormal3f(dx / len, dy / len, 0.0);
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
  parametric_surface(0.3);
  teddy();

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
