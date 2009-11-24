/**
 * @file main.cpp Initialises the GL pipeline, creates display lists 
 * and renders the scene
 */
#include "base.hpp"
#include "models.hpp"

using namespace cm0304;

bool smooth_teddy(false); // If true, use vertex normals for the teddy
bool steam_init(false); // Whether the steam particles have been initialised
GLuint scene_dl(0U); // Display list for the scene
double dist_sens(0.5); // Distance sensitivity - higher means more sensitive
double yaw_sens(0.6); // Rotation sensitivity - higher means more sensitive
double camera_pos[] = {0, 20, 120}; // Camera position {x, y, z}
double camera_yaw(0.0); // Camera turn
float light0_pos[] = { 20.0,  50.0, 50.0 }; // Coordinates of the light source
float spout[3] = {7.3, 8.1, 0}; // Position of the teapot spout
// Idea based on `Glut game keyboard example' from 
// http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/
bool keys[512] = {false}; // Store state of each key

// From shaded.cc
// Initialise the light source
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
// Set the position of the light source
void lights()
{
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
}

// Q.1 (a) easily add/remove objects
// Render the scene. This should be called after glNewList() and glEndList()
// Any objects should be added here
void scene()
{
  glEnable(GL_SMOOTH);
  glBegin(GL_POINTS);
  glPointSize(2.0);
  glVertex3fv(light0_pos);
  glEnd();

  // Draw the teapot and its steam
  draw_teapot();

  // Add a floor
  floor(100, 100, 0);

  // Draw the red teddy
  draw_teddy_one(smooth_teddy);

  // Draw the green teddy
  draw_teddy_two(smooth_teddy);

  // Draw the parametric surface
  parametric_surface(3.0);
}

// Display callback function - render the scene display list and update the
// camera
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
  if (!steam_init)
  {
    init_steam(spout);
    steam_init = true;
  }
  draw_steam(spout);
  glFlush();
  glutSwapBuffers();
}

// From wireframe.cc
void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, static_cast<double>(w)/static_cast<double>(h), 
                 0.1, 400.0);
  glMatrixMode(GL_MODELVIEW);
}

// Handle any keyboard input and update the camera appropriately
// Q.1 (b) A navigation system which allows the user to display the scene
// from an arbitrary position.
// w: forwards
// a: rotate left
// s: backwards
// d: rotate right
// e: up
// q: down
// z: strafe left
// c: strafe right
void keyboard_move()
{
  if (keys['w']) 
  {
    // Forwards
    camera_pos[0] += dist_sens * sin(camera_yaw * deg_to_rad);
    camera_pos[2] -= dist_sens * cos(camera_yaw * deg_to_rad);
  }
  if (keys['s'])
  {
    // Backwards
    camera_pos[0] -= dist_sens * sin(camera_yaw * deg_to_rad);
    camera_pos[2] += dist_sens * cos(camera_yaw * deg_to_rad);
  }
  if (keys['z'])
  {
    // Strafe left
    camera_pos[0] += dist_sens * sin((camera_yaw - 90) * deg_to_rad);
    camera_pos[2] -= dist_sens * cos((camera_yaw - 90) * deg_to_rad);
  }
  if (keys['c'])
  {
    // Strafe right
    camera_pos[0] += dist_sens * sin((camera_yaw + 90) * deg_to_rad);
    camera_pos[2] -= dist_sens * cos((camera_yaw + 90) * deg_to_rad);
  }
  if (keys['a']) 
  {
    // Rotate left
    camera_yaw -= yaw_sens;
  }
  if (keys['d']) 
  {
    // Rotate right
    camera_yaw += yaw_sens;
  }
  if (keys['e'])
  {
    camera_pos[1] += dist_sens;
  }
  if (keys['q'])
  {
    // Don't let the camera go below floor level
    if (camera_pos[1] > dist_sens)
      camera_pos[1] -= dist_sens;
  }
}

// Idea from http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/
void keyboard(unsigned char key, int, int)
{
  keys[key] = true;
}

void keyboard_up(unsigned char key, int, int)
{
  keys[key] = false;
}

// Q.1 (a) Continuously render the scene
void idle()
{
  keyboard_move();
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(600, 400);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("Tea party with weird shape");

  // Set callback functions
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboard_up);

  // Background colour is black
  glClearColor(0.2, 0.2, 0.2, 1.0);
  // Enable visible surface detection via depth tests
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);
  glEnable (GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  // Initialise the lights
  init_lights();

  // Q.1 (a) sets up the scene using display lists
  // Create a display list for the scene
  scene_dl = glGenLists(1);
  glNewList(scene_dl, GL_COMPILE);
  scene();
  glEndList();

  glutMainLoop();
  return 0;
}
