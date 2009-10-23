/**
 * @file models.cpp Implementation of model objects which can be added to the
 * scene.
 */
#include "models.hpp"
#include "util.hpp"

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
