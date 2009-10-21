/**
 * CM0304 coursework 2009
 * Copyright 2009 Alex Duller
 */
#include <iostream>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif /* __APPLE__ */

// Conversion factors for degrees to radians
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

void special_callback(int key, int, int)
{
  g_keys[key] = true;
}

void special_up_callback(int key, int, int)
{
  g_keys[key] = false;
}

void teapot()
{
  // Teapot material
  static GLfloat diffuse[] = {0.0, 0.7, 0.0, 1.0};
  static GLfloat ambient[] = {0.0, 0.5, 0.0, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  // Push current modelview matrix on a matrix stack to save current
  // transformation.
  glPushMatrix ();

  // Multiply modelview matrix with additional transformation to move
  // and rotate the teapot to a different position.
  glTranslated (5.0, 1.0, 0.0);
  glRotated (0.0, 1.0, 0.0, 0.0);
  
  // Glut function for drawing a solid teapot
  glutSolidTeapot (1.0);

  // Get original matrix back from stack (undo above transformation
  // for objects drawn after this one)
  glPopMatrix ();

  return;
}

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
      glNormal3d (sin (deg_to_rad * thet) * cos (deg_to_rad * phi),
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

void floor()
{
  static GLfloat diffuse[] = {0.7, 0.7, 0.0, 1.0};
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

  // Multiply modelview matrix with additional transformation to move
  // and rotate the teapot to a different position.
  glTranslated (0.0, 0.0, 0.0);
  glRotated (0.0, 1.0, 0.0, 0.0);
  
  glBegin(GL_QUADS);
    glVertex3f(-30, 0, -30);
    glVertex3f(-30, 0, 30);
    glVertex3f(30, 0, 30);
    glVertex3f(30, 0, -30);
  glEnd();

  // Get original matrix back from stack (undo above transformation
  // for objects drawn after this one)
  glPopMatrix ();
}

// Add objects to the scene
void scene()
{
  floor();
  sphere();
  teapot();

  return;
}

/**
 * Initialise the lights for the scene
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

void display_callback(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Camera position
  glRotatef(angle, 0, 1, 0);
  glTranslatef(position[0], position[1], position[2]);

  lights();

  glCallList(scene_dl);

  glFlush();

  glutSwapBuffers();
}

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

  glutDisplayFunc(display_callback);
  glutReshapeFunc(reshape_callback);
  glutSpecialFunc(special_callback);
  glutSpecialUpFunc(special_up_callback);
  glutIdleFunc(idle_callback);

  init_lights();

  scene_dl = glGenLists(1);
  glNewList(scene_dl, GL_COMPILE);
  scene();
  glEndList();

  glutMainLoop();

  return 0;
}
