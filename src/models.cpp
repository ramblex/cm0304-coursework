/**
 * @file models.cpp Implementation of model objects which can be added to the
 * scene.
 */
#include "models.hpp"
#include "util.hpp"
#include <cassert>

#include <map>

namespace cm0304
{

using std::map;

vector<triangle_t> m_faces;
vector<vertex_t> m_vertices;

// Whether the teddy mesh has already been read
bool read_teddy_mesh = false;
bool read_cow_mesh = false;

void read_mesh(const string& filename, 
               int num_vertices, 
               int num_faces
               )
{
  ifstream is(filename.c_str());
  std::cout << "Reading mesh from " << filename << "..."; 
  string line = "";
  // @todo Get these from the file
  // Parse the header
  while (getline(is, line))
  {
    if (line == "end_header")
      break;
  }

  // Read in the vertices
  int num_vertices_read = 0;
  while (num_vertices_read < num_vertices)
  {
    getline(is, line);
    stringstream ss(line);
    vertex_t v;
    ss >> v.x >> v.y >> v.z;
    m_vertices.push_back(v);
    ++num_vertices_read;
  }

  // Read in faces and create face points (centroids of the triangle)
  int num_faces_read = 0;
  while (num_faces_read < num_faces)
  {
    getline(is, line);
    int num = 0;
    int idx1 = -1;
    int idx2 = -1;
    int idx3 = -1;
    stringstream ss(line);
    ss >> num >> idx1 >> idx2 >> idx3;

    // Store the original face
    triangle_t t;
    t.v1_idx = idx1;
    t.v2_idx = idx2;
    t.v3_idx = idx3;
    m_faces.push_back(t);

    ++num_faces_read;
  }
  is.close();
  std::cout << "done.\n";
}

void draw_cow()
{
  // Assumes the mesh file does not change once the mesh has been read once
  if (!read_cow_mesh)
  {
    read_mesh("cow.ply", 2903, 5804);
    read_cow_mesh = true;
  }

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

  // Draw the cow
  glPushMatrix();
  // glTranslated(-20, 20, -20);
  // glRotated(45, 0, 1, 0);
  draw_mesh();
  glPopMatrix();
}

void draw_teddy()
{
  // Assumes the mesh file does not change once the mesh has been read once
  if (!read_teddy_mesh)
  {
    read_mesh("teddy.ply", 202, 400);
    read_teddy_mesh = true;
  }

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

  // Draw the teddy
  glPushMatrix();
  glTranslated(-20, 20, -20);
  glRotated(45, 0, 1, 0);
  draw_mesh();
  glPopMatrix();
}

void draw_mesh()
{
  glEnable(GL_NORMALIZE);
  glBegin(GL_TRIANGLES);
  vector<triangle_t>::iterator tit = m_faces.begin();
  for ( ; tit != m_faces.end(); ++tit)
  {
    vertex_t v1 = m_vertices[(*tit).v1_idx];
    vertex_t v2 = m_vertices[(*tit).v2_idx];
    vertex_t v3 = m_vertices[(*tit).v3_idx];
    vertex_t normal = find_normal(v1, v2, v3);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
  }
  glEnd();
}

/**
 * Q2. 3D objects
 */
void vertex_normals_teddy()
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

//     // Assume that the vertices of the triangle are given in an anti-clockwise
//     // direction.
//     vertex_t normal = find_normal(vertices[v1], vertices[v2], vertices[v3]);

//     // Accumulate the normals for each vertex. 
//     if (vector_normals.find(v1) != vector_normals.end())
//       vector_normals[v1] = vector_normals[v1] + normal;
//     else
//       vector_normals[v1] = normal;

//     if (vector_normals.find(v2) != vector_normals.end())
//       vector_normals[v2] = vector_normals[v2] + normal;
//     else
//       vector_normals[v2] = normal;

//     if (vector_normals.find(v3) != vector_normals.end())
//       vector_normals[v3] = vector_normals[v3] + normal;
//     else
//       vector_normals[v3] = normal;

//     triangle_t t;
//     t.v1 = vertices[v1];
//     t.v2 = vertices[v2];
//     t.v3 = vertices[v3];
//     t.normal = normal;
//     triangles.push_back(t);

//     ++num_faces_read;
//   }

//   // Draw the teddy as triangles where each vertex has its own normal
//   glBegin(GL_TRIANGLES);
//   vector<triangle_t>::iterator it = triangles.begin();
//   for ( ; it != triangles.end(); ++it)
//   {
//     triangle_t t = (*it);
//     glNormal3f(t.v1.normal.x, t.v1.normal.y, t.v1.normal.z);
//     glVertex3f(t.v1.x, t.v1.y, t.v1.z);
//     glNormal3f(t.v2.normal.x, t.v2.normal.y, t.v2.normal.z);
//     glVertex3f(t.v2.x, t.v2.y, t.v2.z);
//     glNormal3f(t.v3.normal.x, t.v3.normal.y, t.v3.normal.z);
//     glVertex3f(t.v3.x, t.v3.y, t.v3.z);
//   }
//   glEnd();

//   glPopMatrix();
}

/**
 * Draw a trumpet shaped parametric surface. 
 * Equations are from http://www.math.uri.edu/~bkaskosz/flashmo/tools/parsur/
 * @param res The accuracy with which to draw the parametric surface;
 * the lower, the more accurate.
 */
void parametric_surface(double res)
{
  // Material
  static GLfloat diffuse[] = {0.0, 0.0, 0.7, 1.0};
  static GLfloat ambient[] = {0.0, 0.0, 0.5, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  glPushMatrix ();
  // Push current modelview matrix on a matrix stack to save current
  // transformation.
  glEnable(GL_NORMALIZE);
  glTranslated(15, 15, 5);
  glRotated(125, 0, 1, 0);

  for (double s = -180; s <= 180; s += res)
  {
    glBegin(GL_QUAD_STRIP);
    for (double t = 0; t < 17; ++t)
    {
      vertex_t v0;
      v0.x = t;
      v0.y = 6/pow(t + 1, 0.7) * cos(s * deg_to_rad);
      v0.z = 6/pow(t + 1, 0.7) * sin(s * deg_to_rad);

      vertex_t v1;
      v1.x = t;
      v1.y = 6/pow(t + 1, 0.7) * cos((s + res) * deg_to_rad);
      v1.z = 6/pow(t + 1, 0.7) * sin((s + res) * deg_to_rad);

      // Use a third point to get two edges on the face
      vertex_t v2;
      v2.x = t+1;
      v2.y = 6/pow(t + 2, 0.7) * cos((s + res) * deg_to_rad);
      v2.z = 6/pow(t + 2, 0.7) * sin((s + res) * deg_to_rad);

      vertex_t normal = find_normal(v0, v1, v2);
      glNormal3d(normal.x, normal.y, normal.z);
      glVertex3d(v0.x, v0.y, v0.z);
      glVertex3d(v1.x, v1.y, v1.z);
    }
    glEnd();
  }

  // Get original matrix back from stack (undo above transformation
  // for objects drawn after this one)
  glPopMatrix ();
}

// Draw the floor of the scene
void floor(double width, double depth, double pos)
{
  // Material
  static GLfloat diffuse[] = {0.0, 0.7, 0.0, 1.0};
  static GLfloat ambient[] = {0.8, 0.8, 0.8, 1.0};
  static GLfloat specular[] = {0.0, 0.0, 0.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  glBegin(GL_QUADS);
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(-width, pos, depth);
  glVertex3f(width, pos, depth);
  glVertex3f(width, pos, -depth);
  glVertex3f(-width, pos, -depth);
  glEnd();
}
}  // namespace cm0304
