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

void Teddy::read_mesh()
{
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
  int num_vertices_read = 0;
  while (num_vertices_read < num_vertices)
  {
    getline(is, line);
    stringstream ss(line);
    cm_vertex_t v;
    ss >> v.position.x >> v.position.y >> v.position.z;
    m_vertices.push_back(v);
    ++num_vertices_read;
  }

  std::cout << "Read in vertices" << std::endl;

  // Read in faces and create face points (centroids of the triangle)
  vector<triangle_t> triangles;
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

  std::cout << "Read in faces" << std::endl;
}

void Teddy::subdivide()
{
  // unsigned int num_faces = m_faces.size();
  // unsigned int new_num_faces = num_faces * 4;
  // unsigned int num_points = m_vertices.size();
  // unsigned int new_num_points = num_points + m_edges.size() + num_faces;
  
  // m_vertices.resize(new_num_points);
  // m_faces.resize(new_num_points);

  // vector<triangle_t>::iterator it = m_faces.begin();
  // for ( ; it != m_faces.end(); ++it)
  // {
  //   cm_vertex_t a = m_vertices[(*it).v1_idx];
  //   cm_vertex_t b = m_vertices[(*it).v2_idx];
  //   cm_vertex_t c = m_vertices[(*it).v3_idx];

  //   // Add a face point 
  //   // vertex_t face_point = (v1.position + v2.position + v3.position) / 3;

  //   // For each edge add an edge point (a + b + f1 + f2) / 4

  // }

  // Move original points


    // cm_vertex_t& v1 = m_vertices[idx1];
    // cm_vertex_t& v2 = m_vertices[idx2];
    // cm_vertex_t& v3 = m_vertices[idx3];

    // // Add indexes of adjacent vertices
    // v1.adj_verts.insert(idx2);
    // v1.adj_verts.insert(idx3);
    // v2.adj_verts.insert(idx1);
    // v2.adj_verts.insert(idx3);
    // v3.adj_verts.insert(idx1);
    // v3.adj_verts.insert(idx2);

    // // Triangle centroid
    // vertex_t face_point = (v1.position + v2.position + v3.position) / 3;
    // v1.centroids.push_back(face_point);
    // v2.centroids.push_back(face_point);
    // v3.centroids.push_back(face_point);

    // // Create edge midpoints
    // vertex_t mid1 = (v1.position + v2.position) / 2;
    // vertex_t mid2 = (v2.position + v3.position) / 2;
    // vertex_t mid3 = (v3.position + v1.position) / 2;

    // v1.edge_mids.insert(mid1);
    // v2.edge_mids.insert(mid1);
    // v2.edge_mids.insert(mid2);
    // v3.edge_mids.insert(mid2);
    // v3.edge_mids.insert(mid3);
    // v1.edge_mids.insert(mid1);
  // Move original vertices
  vector<cm_vertex_t>::iterator it = m_vertices.begin();
  for ( ; it != m_vertices.end(); ++it)
  {
    set<vertex_t>::iterator edge_it = (*it).edge_mids.begin();
    vertex_t edge_mids_sum(0, 0, 0);
    for ( ; edge_it != (*it).edge_mids.end(); ++edge_it)
    {
      edge_mids_sum += *edge_it;
    }
    vector<vertex_t>::iterator face_it = (*it).centroids.begin();
    vertex_t centroids_sum(0, 0, 0);
    for ( ; face_it != (*it).centroids.end(); ++face_it)
    {
      centroids_sum += *face_it;
    }

    double n = (*it).adj_verts.size();
    assert(n == (*it).centroids.size());
    vertex_t Q = centroids_sum / n;
    vertex_t R = edge_mids_sum / n;
    vertex_t& S = (*it).position;
    //    std::cout << "Q = " << Q << ", R = " <<  R << std::endl;
    std::cout << "n = "  << n << std::endl;
    std::cout << "original = " << S;
    (*it).position = (Q + R * 2 + S * (n - 3)) / n;
    std::cout << ", new = " << (*it).position << std::endl;
  }
}

void Teddy::set_material()
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
}

void Teddy::draw()
{
  //  set_material();
  if (!m_read_mesh)
  {
    std::cout << "Reading mesh...";
    read_mesh();
    m_read_mesh = true;
    std::cout << "done.\n";
  }

  // int num_times_subdivided = 0;
  // while (num_times_subdivided < m_num_subdivides)
  //   subdivide();

  // Draw the teddy
  glPushMatrix();
  glEnable(GL_NORMALIZE);
  glBegin(GL_TRIANGLES);
  vector<triangle_t>::iterator tit = m_faces.begin();
  for ( ; tit != m_faces.end(); ++tit)
  {
    cm_vertex_t v1 = m_vertices[(*tit).v1_idx];
    cm_vertex_t v2 = m_vertices[(*tit).v2_idx];
    cm_vertex_t v3 = m_vertices[(*tit).v3_idx];
    vertex_t normal = find_normal(v1.position, v2.position, v3.position);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v1.position.x, v1.position.y, v1.position.z);
    glVertex3f(v2.position.x, v2.position.y, v2.position.z);
    glVertex3f(v3.position.x, v3.position.y, v3.position.z);
  }
  glEnd();
  glPopMatrix();
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

//   ifstream is("teddy.ply");
//   string line = "";
//   const int num_vertices = 202; // Number of vertices in the mesh
//   const int num_faces = 400; // Number of faces in the mesh
//   // Parse the header
//   while (getline(is, line))
//   {
//     std::cout << "Reading line" << std::endl;
//     if (line == "end_header")
//       break;
//   }

//   // Read in the vertices
//   vector<vertex_t> vertices;
//   int num_vertices_read = 0;
//   while (num_vertices_read < num_vertices)
//   {
//     getline(is, line);
//     stringstream ss(line);
//     vertex_t v;
//     ss >> v.x >> v.y >> v.z;
//     vertices.push_back(v);
//     ++num_vertices_read;
//   }

//   glPushMatrix();
//   glEnable(GL_NORMALIZE);
//   glScalef(0.2, 0.2, 0.2);
//   glTranslated(30.0, 20.0, 30.0);
//   glTranslated(-10.0, 0.0, 0.0);
//   // Read in the faces and compute the face normals, storing them in a vector.
//   // This makes it easy to do work out the vector normals.
//   int num_faces_read = 0;
//   vector<vertex_t> face_normals;
//   map<int, vertex_t> vector_normals;
//   vector<triangle_t> triangles;
//   while (num_faces_read < num_faces)
//   {
//     getline(is, line);
//     stringstream ss(line);
//     int num = 0; // Number of vertices - this is discarded
//     int v1 = -1;
//     int v2 = -1;
//     int v3 = -1;
//     // Parse the line into doubles
//     ss >> num >> v1 >> v2 >> v3;

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
  glPushMatrix ();

  // Push current modelview matrix on a matrix stack to save current
  // transformation.
  glEnable(GL_NORMALIZE);
  glTranslated (30.0, 0.0, 0.0);
  //  glScaled(0.5, 0.5, 0.5);
  //  glRotated(45.0, 0.0, 45.0, 1.0);

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
  glBegin(GL_QUADS);
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(-width, pos, depth);
  glVertex3f(width, pos, depth);
  glVertex3f(width, pos, -depth);
  glVertex3f(-width, pos, -depth);
  glEnd();
}
}  // namespace cm0304
