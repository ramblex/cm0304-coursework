#include "base.hpp"
#include "models.hpp"

using namespace cm0304;

bool steam_init = false;

GLuint scene_dl(0U);
GLuint scene_obj(0U);

double dist_sens = 5;
double yaw_sens = 5;

double camera_pos[] = {0, 20, 120};
double camera_yaw(0.0);
double camera_pitch(0.0);

double scene_yaw = 0;
double scene_pitch = 0;

float light0_pos[] = { 0.0,  80.0, 0.0 }; // Coordinates of the light source

// If true, use vertex normals for the teddy
bool smooth_teddy = false;

/*
 * This is copied from http://www.devmaster.net/articles/shadowprojection/
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

void axes(void)
{
  glPushMatrix ();
  glTranslatef (camera_pos[0]-2.4, camera_pos[1], camera_pos[2]-5);
  glRotated(scene_yaw, 0, 1, 0);
  glRotated(scene_pitch, 0, 0, 1);
  glScalef (0.25, 0.25, 0.25);
  glLineWidth (2.0);
  glBegin (GL_LINES);
  float origin[3] = {0,0,0};
  glColor3f(1,0,0); // X axis is red.
  glVertex3fv(origin);
  glVertex3f(1, 0, 0); 
  glColor3f(0,1,0); // Y axis is green.
  glVertex3fv(origin);
  glVertex3f(0, 1, 0);
  glColor3f(0,0,1); // z axis is blue.
  glVertex3fv(origin);
  glVertex3f(0, 0, 1); 
  glEnd();
  glPopMatrix ();
}

// From shaded.cc
void init_lights()
{
  // // Specify light emitted by light source 0
  static GLfloat diffuse[] = {0.5, 0.5, 0.5, 1.0};
  static GLfloat ambient[] = {0.1, 0.1, 0.1, 1.0};
  static GLfloat specular[] = {0.8, 0.8, 0.8, 1.0};
  static GLfloat attenuation = 1;

  // Enable lighting
  glEnable (GL_LIGHTING);

  // Enable first light source
  glEnable (GL_LIGHT0);
  // Set light emitted by light source 0
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv (GL_LIGHT0, GL_CONSTANT_ATTENUATION, &attenuation);
}

// From shaded.cc
void lights()
{
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
}

void scene_objects()
{
  draw_teddy_one(smooth_teddy);
  draw_teddy_two(smooth_teddy);
  parametric_surface(3.0);
  draw_teapot();
}

void scene()
{
  // Add the light as a point for debugging purposes 
  // glBegin(GL_POINTS);
  // //  glColor3f(1.0, 1.0, 1.0);
  // glVertex3fv(light0_pos);
  // glEnd();

  // Add a floor
  floor(100, 100, 0);

  glCallList(scene_obj);

  // Draw shadows
  float n[] = { 0.0,  -1.0, 0.0 }; // Normal vector for the plane
  float e[] = { 0.0, 0.1, 0.0 }; // Point of the plane
  glPushMatrix();
  // glRotatef(ry,0,1,0);
  // glRotatef(rx,1,0,0);
  glDisable(GL_LIGHTING);
  glShadowProjection(light0_pos, e, n);
  glColor3f(0.0,0.0,0.0);
  glCallList(scene_obj);
  glPopMatrix();

  glEnable(GL_LIGHTING);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2],
            camera_pos[0] + sin(camera_yaw * deg_to_rad), 
            camera_pos[1], 
            camera_pos[2] - cos(camera_yaw * deg_to_rad),
            0.0, 1.0, 0.0);
  lights();
  glCallList(scene_dl);
  // Draw the steam
  float spout[3] = {7.3, 7.5, 0};
  if (!steam_init)
  {
    init_steam(spout);
    steam_init = true;
  }
  draw_steam(spout);
  glFlush();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (60.0, static_cast<double>(w)/static_cast<double>(h), 
                  0.1, 400.0);
  glMatrixMode (GL_MODELVIEW);
}

void idle()
{
  glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
  if (key == 'w') 
  {
    // Forwards
    camera_pos[0] += dist_sens * sin(camera_yaw * deg_to_rad);
    camera_pos[2] -= dist_sens * cos(camera_yaw * deg_to_rad);
  }
  else if (key == 's')
  {
    // Backwards
    camera_pos[0] -= dist_sens * sin(camera_yaw * deg_to_rad);
    camera_pos[2] += dist_sens * cos(camera_yaw * deg_to_rad);
  }
  else if (key == 'q')
  {
    // Strafe left
    camera_pos[0] += dist_sens * sin((camera_yaw - 90) * deg_to_rad);
    camera_pos[2] -= dist_sens * cos((camera_yaw - 90) * deg_to_rad);
  }
  else if (key == 'e')
  {
    // Strafe right
    camera_pos[0] += dist_sens * sin((camera_yaw + 90) * deg_to_rad);
    camera_pos[2] -= dist_sens * cos((camera_yaw + 90) * deg_to_rad);
  }
  else if (key == 'a') 
  {
    // Rotate left
    camera_yaw -= yaw_sens;
  }
  else if (key == 'd') 
  {
    // Rotate right
    camera_yaw += yaw_sens;
  }
  else if (key == 'r')
  {
    camera_pos[1] += dist_sens;
  }
  else if (key == 'f')
  {
    camera_pos[1] -= dist_sens;
  }
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize (600, 400);
  glutInitWindowPosition (400, 300);
  glutInitDisplayMode (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow ("Tea party with weird shape");
  glutDisplayFunc (display);
  glutIdleFunc (idle);
  glutReshapeFunc (reshape);
  glutKeyboardFunc(keyboard);
  //  glutSpecialFunc (Turn);

  //  glEnable(GL_COLOR_MATERIAL);

  glClearColor (0.0, 0.0, 0.0, 1.0);
  // Enable visible surface detection via depth tests
  glDepthFunc (GL_LEQUAL);
  glClearDepth (1.0);
  glEnable (GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  init_lights();

  scene_obj = glGenLists(1);
  glNewList(scene_obj, GL_COMPILE);
  scene_objects();
  glEndList();

  scene_dl = glGenLists(1);
  glNewList(scene_dl, GL_COMPILE);
  scene();
  glEndList();

  glutMainLoop ();
  return 0;
}
