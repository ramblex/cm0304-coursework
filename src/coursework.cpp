/**
 * @file coursework.cpp
 * CM0304 coursework 2009
 *
 * Implements a basic 3D OpenGL scene which can be navigated in a walk-through
 * fashion.
 *
 * Copyright 2009 Alex Duller
 */
#include "base.hpp"
#include "models.hpp"

// ID of display list containing scene description
GLuint scene_dl(0U);

// The y rotation of the camera in degrees
double angle = 0;

// If true, use subdivision for the teddy 
bool smooth_teddy = false;

// Camera sensitivity
const double rotate_sensitivity = 0.4;
const double dist_sensitivity = 0.1;

// The inverted position of the camera
double position[3] = {0, -2, -30};

/** Used to keep track of keyboard keys - this is so we can use multiple keys
 * at once. */
int g_keys[128] = {false};

/** Handle special keys when they are pressed (idea from ...) */
void special_callback(int key, int, int)
{
  g_keys[key] = true;
  return;
}

// Unset the special key flag.
void special_up_callback(int key, int, int)
{
  g_keys[key] = false;
  return;
}

void keyboard_callback(unsigned char key, int, int)
{
}

/**
 * Add objects to the scene. Order does not matter since we are using
 * DEPTH_TEST. They will be automatically added to the scene's display list
 */
void scene()
{
  cm0304::floor();
  cm0304::vertex_normals_teddy();
  cm0304::Teddy t(0);
  t.draw();
  cm0304::parametric_surface(20);
  return;
}

/**
 * Initialise the lights for the scene. Adapted from shaded.cc
 */
void init_lights()
{
  // Enable lighting
  glEnable(GL_LIGHTING);

  // Fill in polygons
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

    // Enable first light 
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
  static GLfloat light0_pos[] = {20.0, 20.0, 10.0, 1.0f};
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
  glutKeyboardFunc(keyboard_callback);

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
