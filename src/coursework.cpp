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

// The y rotation of the camera in degrees
float angle = 0;

// Rate at which camera is rotated, higher means faster rotation
float rotate_sensitivity = 0.4;

float dist_sensitivity = 0.1;

// The inverted position of the camera
float position[3]={0, -2, 0};

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

void display_callback(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // Camera position
  glRotatef(angle, 0, 1, 0);
  glTranslatef(position[0], position[1], position[2]);

  // Draw a grid
  glBegin(GL_LINES);
  for (int i = -30; i <= 30; ++i)
  {
    glVertex3f(i, 0, -30);
    glVertex3f(i, 0, 30);
    glVertex3f(30, 0, i);
    glVertex3f(-30, 0, i);
  }
  glEnd();

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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Grid");

  glutDisplayFunc(display_callback);
  glutReshapeFunc(reshape_callback);
  glutSpecialFunc(special_callback);
  glutSpecialUpFunc(special_up_callback);
  glutIdleFunc(idle_callback);

  glutMainLoop();

  return 0;
}
