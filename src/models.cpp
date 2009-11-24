/**
 * @file models.cpp Implementation of model objects which can be added to the
 * scene. Function comments are in the header
 */
#include "models.hpp"

namespace cm0304
{
// Texture for the steam particles
GLuint texture[1];

// Particles used for the steam
vector<particle_t> particles(knum_particles);

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
  glTranslated(-50, 40, 20);
  glScalef(3.0, 3.0, 3.0);
  glRotated(45, 0, 0, 1);

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

void draw_teddy(bool use_vertex_normals)
{
  std::cerr << "Reading mesh..."; 
  ifstream is("teddy.ply");
  string line = "";

  const int num_vertices = 202;
  const int num_faces = 400;

  vector<triangle_t> m_faces;
  vector<vertex_t> vertices;
  vector<vertex_t> vertex_normals;

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
    vertices.push_back(v);
    ++num_vertices_read;
  }

  std::cerr << "done reading vertices" << std::endl;

  // Read in faces and create face points (centroids of the triangle)
  int num_faces_read = 0;
  while (num_faces_read < num_faces)
  {
    getline(is, line);
    int num = 0;
    stringstream ss(line);
    triangle_t t;
    ss >> num >> t.v1_idx >> t.v2_idx >> t.v3_idx;

    vertex_t& v1 = vertices[t.v1_idx];
    vertex_t& v2 = vertices[t.v2_idx];
    vertex_t& v3 = vertices[t.v3_idx];

    vertex_t normal = find_normal(v1, v2, v3);
    if (use_vertex_normals)
    {
      // Accumulate the normals of the adjacent faces for each vertex
    }

    t.normal = normal;

    // Store the original face
    m_faces.push_back(t);
    ++num_faces_read;
    std::cout << "read face" << std::endl;
  }
  is.close();
  std::cout << "done.\n";

  glEnable(GL_NORMALIZE);
  glBegin(GL_TRIANGLES);
  vector<triangle_t>::iterator tit = m_faces.begin();
  for ( ; tit != m_faces.end(); ++tit)
  {
    vertex_t v1 = vertices[(*tit).v1_idx];
    vertex_t v2 = vertices[(*tit).v2_idx];
    vertex_t v3 = vertices[(*tit).v3_idx];

    vertex_t normal = (*tit).normal;
    // Just use the face normal if vertex normals are turned off
    if (!use_vertex_normals)
      glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
  }
  glEnd();
}

void draw_teddy_one(bool use_vertex_normals)
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

  // Draw the teddy
  glPushMatrix();
  glTranslated(-20, 20, -20);
  glRotated(45, 0, 1, 0);
  draw_teddy(use_vertex_normals);
  glPopMatrix();
}

void draw_teddy_two(bool use_vertex_normals)
{
  // Material
  static GLfloat diffuse[] = {0.0, 0.7, 0.0, 1.0};
  static GLfloat ambient[] = {0.0, 0.5, 0.0, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  // Draw the teddy
  glPushMatrix();
  glTranslated(20, 20, -20);
  glRotated(-45, 0, 1, 0);
  draw_teddy(use_vertex_normals);
  glPopMatrix();
}

void init_steam_particle(particle_t& p, vertex_t& start_pos)
{
  p.is_dead = false;
  p.brightness = 1.0;
  p.fade_amount = (double(rand() % 100) / 1000) + 0.0009;
  p.size = 1;
  // Start the particles at the teapot spout
  p.pos = start_pos;
  p.speed[0] = 0.0003-(double(rand() % 100) / 10000);
  p.speed[1] = 0.02-(double(rand() % 100) / 10000);
  p.speed[2] = 0.0003-(double(rand() % 100) / 10000);
  p.rotation = double(rand() % 100) / 100;
}

void load_particle_texture()
{
  // http://www.gamedev.net/community/forums/topic.asp?topic_id=445226
  int w = 32;
  int h = 32;
  int s=w*h*3;
  GLubyte bits[s];
  FILE *file;
  file=fopen("textures/raw_particle.raw","rb");
  assert(file != NULL);
  fread(&bits,s,1,file);
  fclose(file);
  glGenTextures(1,&texture[0]);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D ,0 , 3, w, h,0,GL_RGB,GL_UNSIGNED_BYTE,bits);
}

void init_steam(float spout[3])
{
  load_particle_texture();
  // Offset the start height to prevent too much overlap particles
  vertex_t start_pos(spout[0], 
                     spout[1] + (double(rand() % 100)), 
                     spout[2]);

  // Initialise the particles
  vector<particle_t>::iterator it = particles.begin();
  for ( ; it != particles.end(); ++it)
  {
    init_steam_particle(*it, start_pos);
  }
}

/**
 * Advanced feature. Implement a particle effect such as steam from a teapot.
 * @param spout Location of the spout
 */
// Ideas from http://www.gamedev.net/community/forums/topic.asp?topic_id=392601&whichpage=1&#2601399
// Some very basic help from http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=19 (e.g. turning on blend)
void draw_steam(float spout[3])
{
  glShadeModel(GL_SMOOTH);
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  // Offset the start height to prevent too much overlap particles
  vertex_t start_pos(spout[0], 
                     spout[1] + (double(rand() % 100) / 50), 
                     spout[2]);
  // Draw the steam
  vector<particle_t>::iterator it = particles.begin();
  for ( ; it != particles.end(); ++it)
  {
    // Reinitialise any dead particles
    if ((*it).is_dead)
      init_steam_particle(*it, start_pos);
    // Move the particle upwards, rotate it, increase its size and fade it out
    (*it).pos.x += (*it).speed[0];
    (*it).pos.y += (*it).speed[1];
    (*it).pos.z += (*it).speed[2];
    (*it).brightness -= (*it).fade_amount;
    (*it).size += (*it).speed[1];
    (*it).rotation += (*it).speed[0] * 10;

    static GLfloat diffuse[] = {0.1, 0.1, 0.1, (*it).brightness};
    static GLfloat ambient[] = {0.3, 0.3, 0.3, (*it).brightness};
    static GLfloat specular[] = {0.0, 0.0, 0.0, (*it).brightness};
    static GLfloat shine (127.0);

    // Set material
    glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

    // Draw the particle
    if ((*it).brightness > 0)
    {
      glPushMatrix();
      glTranslated((*it).pos.x - 1, (*it).pos.y - 1, (*it).pos.z - 1);
      glRotated(2 * (*it).rotation, 0, 1, 0);
      glBegin(GL_QUADS);
      glTexCoord2d(0, 0);
      glVertex3f(1, 1, 1);
      glTexCoord2d(1, 0);
      glVertex3f(1+(*it).size, 1, 1);
      glTexCoord2d(1, 1);
      glVertex3f(1+(*it).size, 1-(*it).size, 1);
      glTexCoord2d(0, 1);
      glVertex3f(1, 1-(*it).size, 1);
      glEnd();
      glPopMatrix();
    }
    else
      (*it).is_dead = true;
  }
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}

void draw_teapot()
{
  // Material
  static GLfloat diffuse[] = {0.7, 0.7, 0.0, 1.0};
  static GLfloat ambient[] = {0.7, 0.7, 0.0, 1.0};
  static GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
  static GLfloat shine (127.0);

  // Set material
  glMaterialfv (GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &shine);

  glPushMatrix();
  glTranslated(0, 3.5, 0);
  glScalef(5.0, 5.0, 5.0);
  glutSolidTeapot(1.0);
  glPopMatrix();
}
}  // namespace cm0304
