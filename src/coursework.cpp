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

cm0304::Teddy* t = new cm0304::Teddy(0);

double rx = 0.0;
double ry = 0.0;

float l[] = { 0.0,  80.0, 0.0 }; // Coordinates of the light source
float n[] = { 0.0,  -1.0, 0.0 }; // Normal vector for the plane
float e[] = { 0.0, -20.0, 0.0 }; // Point of the plane

void help();

// This function is called whenever the object needs to be drawn
// (For the shadow and itself; for each frame twice)
void draw()
{
  t->draw();

  cm0304::parametric_surface(3);
}

/*
 * This is where the "magic" is done:
 *
 * Multiply the current ModelView-Matrix with a shadow-projetion
 * matrix.
 *
 * l is the position of the light source
 * e is a point on within the plane on which the shadow is to be
 *   projected.
 * n is the normal vector of the plane.
 *
 * Everything that is drawn after this call is "squashed" down
 * to the plane. Hint: Gray or black color and no lighting
 * looks good for shadows *g*
 */
void glShadowProjection(float * l, float * e, float * n)
{
  float d, c;
  float mat[16];

  // These are c and d (corresponding to the tutorial)

  d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
  c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;

  // Create the matrix. OpenGL uses column by column
  // ordering

  mat[0]  = l[0]*n[0]+c;
  mat[4]  = n[1]*l[0];
  mat[8]  = n[2]*l[0];
  mat[12] = -l[0]*c-l[0]*d;

  mat[1]  = n[0]*l[1];
  mat[5]  = l[1]*n[1]+c;
  mat[9]  = n[2]*l[1];
  mat[13] = -l[1]*c-l[1]*d;

  mat[2]  = n[0]*l[2];
  mat[6]  = n[1]*l[2];
  mat[10] = l[2]*n[2]+c;
  mat[14] = -l[2]*c-l[2]*d;

  mat[3]  = n[0];
  mat[7]  = n[1];
  mat[11] = n[2];
  mat[15] = -d;

  // Finally multiply the matrices together *plonk*
  glMultMatrixf(mat);
}

/**
 * render - gets called whenever we want to redraw the scene
 */
void render()
{
  glClearColor(0.0,0.6,0.9,0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glLightfv(GL_LIGHT0, GL_POSITION, l);

  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);

  glColor3f(1.0,1.0,0.0);
  glBegin(GL_POINTS);
  glVertex3f(l[0],l[1],l[2]);
  glEnd();

  // First, we draw the plane onto which the shadow should fall
  // The Y-Coordinate of the plane is reduced by 0.1 so the plane is
  // a little bit under the shadow. We reduce the risk of Z-Buffer
  // flittering this way.
  glColor3f(0.8,0.8,0.8);
  cm0304::floor(1300, 1300, e[1]-0.1);

  // Draw the object that casts the shadow
  glPushMatrix();
  // glRotatef(ry,0,1,0);
  // glRotatef(rx,1,0,0);
  glEnable(GL_LIGHTING);
  glColor3f(0.0,0.0,0.8);
  t->draw();
  glColor3f(0.0,0.8,0.8);
  cm0304::parametric_surface(3.0);
  glPopMatrix();

  // Now we draw the shadow
  glPushMatrix();
  glShadowProjection(l,e,n);
  // glRotatef(ry,0,1,0);
  // glRotatef(rx,1,0,0);
  glDisable(GL_LIGHTING);
  glColor3f(0.4,0.4,0.4);
  t->draw();
  cm0304::parametric_surface(3.0);
  glPopMatrix();

  glutSwapBuffers();
}

void keypress(unsigned char c, int, int)
{
  if ( c==27 ) exit(0);
  else if ( c=='s' ) l[1]-=5.0;
  else if ( c=='w' ) l[1]+=5.0;
  else if ( c=='a' ) l[0]-=5.0;
  else if ( c=='d' ) l[0]+=5.0;
  else if ( c=='q' ) l[2]-=5.0;
  else if ( c=='e' ) l[2]+=5.0;
  else if ( c=='?' ) help();
}

void help()
{
  printf("=======================================================\n");
  printf("Shadow projection example by Phaetos <phaetos@gaffga.de>\n");
  printf("-------------------------------------------------------\n");
  printf("s/w        Move light source up / dow\n");
  printf("a/d        Move light source left / right\n");
  printf("q/e        Move light source forward / backward\n");
  printf("?          This help\n");
  printf("=======================================================\n");
}

void idle()
{
  rx+=0.4;
  ry+=0.7;

  render();
}

void resize(int w, int h)
{
  glViewport(0, 0, w, h);
}

int main(int argc, char * argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("shadow projection demo");
  glutReshapeFunc(resize);
  glutReshapeWindow(400,400);
  glutKeyboardFunc(keypress);
  glutDisplayFunc(render);
  glutIdleFunc(idle);

  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_TEXTURE_2D);
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  gluPerspective(60.0f, 1.0, 1.0, 400.0);

  // Reset the coordinate system before modifying
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -150.0);

  help();

  glutMainLoop();

  return 0;
}

